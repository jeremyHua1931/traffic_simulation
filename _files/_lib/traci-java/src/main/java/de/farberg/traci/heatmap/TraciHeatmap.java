package de.farberg.traci.heatmap;

import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.util.concurrent.TimeUnit;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVPrinter;
import org.apache.log4j.Level;
import org.apache.log4j.chainsaw.Main;
import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.kohsuke.args4j.Option;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.base.Ticker;
import com.google.common.cache.Cache;
import com.google.common.cache.CacheBuilder;
import com.google.common.collect.HashBasedTable;
import com.google.common.collect.Table;

import de.farberg.traci.util.CommandLineOptionsTemplate;
import de.farberg.traci.util.LoggingUtil;
import it.polito.appeal.traci.SumoTraciConnection;
import it.polito.appeal.traci.Vehicle;

public class TraciHeatmap {
	static {
		LoggingUtil.setupLogging();
	}

	public static class SimulationTimeTicker extends Ticker {
		private long currentTime = 0;

		@Override
		public long read() {
			return currentTime;
		}

		public void set(long time) {
			this.currentTime = time;
		}

	};

	private static final SimulationTimeTicker ticker = new SimulationTimeTicker();

	public static class CO2 {
		private Cache<String, Double> cars;

		public CO2(int maxAge, TimeUnit maxAgeTimeUnit) {
			cars = CacheBuilder.newBuilder()
					.concurrencyLevel(1)
					.expireAfterWrite(maxAge, maxAgeTimeUnit)
					.ticker(ticker)
					// .removalListener(listener -> System.out.println("removal of " + listener.getCause()))
					.build();
		}

		void update(String car, double emission) {
			cars.asMap().put(car, emission);
		}

		double get() {
			return cars.asMap().values().stream().mapToDouble(m -> m).sum();
		}

	}

	public static void main(String[] args) throws IllegalStateException, IOException, InterruptedException {
		// Parse command line options
		CommandLineOptions options = parseCmdLineOptions(args);
		Logger log = LoggerFactory.getLogger(Main.class);

		if (options.verbose) {
			org.apache.log4j.Logger.getRootLogger().setLevel(Level.DEBUG);
			log.debug("Debug enabled");
		}

		CSVPrinter csvPrinter = options.csvOutFile == null ? null
				: CSVFormat.EXCEL.withHeader("timestamp", "vehicle-id", "location-x", "location-y", "co2-emission")
						.withDelimiter(',')
						.withQuote('"')
						.print(new FileWriter(options.csvOutFile));

		SumoTraciConnection traci = new SumoTraciConnection(InetAddress.getByName(options.traciAddress), options.traciPort);
		Table<Integer, Integer, CO2> co2Heatmap = HashBasedTable.create(options.xSize, options.ySize);

		for (int i = 0; i < options.iterations; ++i) {
			traci.nextSimStep();
			log.debug("At step {}/{}", i, options.iterations - 1);

			double maxX = -1;
			double maxY = -1;

			for (Vehicle vehicle : traci.getVehicleRepository().getAll().values()) {
				int currentSimTime = traci.getCurrentSimTime();
				ticker.set(currentSimTime);

				double posX = vehicle.getPosition().getX();
				double posY = vehicle.getPosition().getY();
				maxX = Math.max(maxX, posX);
				maxY = Math.max(maxY, posY);

				int xLabel = (int) Math.round((posX / maxX) * options.xSize);
				int yLabel = (int) Math.round((posY / maxY) * options.ySize);

				CO2 co2 = co2Heatmap.get(xLabel, yLabel);
				if (co2 == null) {
					co2 = new CO2(options.maxAgeMillis, TimeUnit.MILLISECONDS);
					co2Heatmap.put(xLabel, yLabel, co2);
				}

				co2.update(vehicle.getID(), vehicle.getCo2Emission());

				if (csvPrinter != null) {
					csvPrinter.printRecord(currentSimTime, vehicle.getID(), xLabel, yLabel, co2.get());
				}

			}
		}

		csvPrinter.flush();
		csvPrinter.close();

		traci.close();
	}

	private static void printHelpAndExit(CmdLineParser parser) {
		System.err.print("Usage: java " + Main.class.getCanonicalName());
		parser.printSingleLineUsage(System.err);
		System.err.println();
		parser.printUsage(System.err);
		System.exit(1);
	}

	private static CommandLineOptions parseCmdLineOptions(final String[] args) {
		CommandLineOptions options = new CommandLineOptions();
		CmdLineParser parser = new CmdLineParser(options);

		try {
			parser.parseArgument(args);
			if (options.help)
				printHelpAndExit(parser);
		} catch (CmdLineException e) {
			System.err.println(e.getMessage());
			printHelpAndExit(parser);
		}

		return options;
	}

	public static class CommandLineOptions extends CommandLineOptionsTemplate {

		@Option(name = "-x", aliases = { "--x-size" }, usage = "", required = false)
		public int xSize = 100;

		@Option(name = "-y", aliases = { "--y-size" }, usage = "", required = false)
		public int ySize = 100;

		@Option(name = "-t", aliases = { "--max-age-ms" }, usage = "", required = false)
		public int maxAgeMillis = 10000;

	}

}
