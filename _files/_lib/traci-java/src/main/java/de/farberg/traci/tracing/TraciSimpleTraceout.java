package de.farberg.traci.tracing;

import java.awt.geom.Point2D;
import java.io.FileWriter;
import java.io.IOException;
import java.net.InetAddress;
import java.util.Map;

import javax.xml.stream.XMLStreamException;

import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVPrinter;
import org.apache.log4j.Level;
import org.apache.log4j.chainsaw.Main;
import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import de.farberg.traci.util.CommandLineOptionsTemplate;
import de.farberg.traci.util.EdgeIdToNameMapping;
import de.farberg.traci.util.LoggingUtil;
import it.polito.appeal.traci.Edge;
import it.polito.appeal.traci.PositionConversionQuery;
import it.polito.appeal.traci.SumoTraciConnection;
import it.polito.appeal.traci.Vehicle;

public class TraciSimpleTraceout {
	static {
		LoggingUtil.setupLogging();
	}

	public static void main(String[] args) throws IllegalStateException, IOException, InterruptedException, XMLStreamException {
		// Parse command line options
		CommandLineOptions options = parseCmdLineOptions(args);
		Logger log = LoggerFactory.getLogger(Main.class);

		if (options.verbose) {
			org.apache.log4j.Logger.getRootLogger().setLevel(Level.DEBUG);
			log.debug("Debug enabled");
		}

		CSVPrinter csvPrinter = options.csvOutFile == null ? null
				: CSVFormat.EXCEL.withHeader("timestamp", "vehicle-id", "vehicle-type", "location-lat", "location-long", "edge-id",
						"edge-name", "speed", "fuel-consumption", "co2-emission", "co-emission", "hc-emission", "noise-emission",
						"nox-emission", "pmx-emission").withDelimiter(',').withQuote('"').print(new FileWriter(options.csvOutFile));

		log.debug("Trying to establish connection to sumo @ {}:{}", options.traciAddress, options.traciPort);
		SumoTraciConnection traci = new SumoTraciConnection(InetAddress.getByName(options.traciAddress), options.traciPort);
		PositionConversionQuery positionConversion = traci.queryPositionConversion();

		// Check if we need to read the net.xml file in order to perform edge id -> name mapping
		EdgeIdToNameMapping edgeToNameMapping = null;
		if (options.netFile != null) {
			edgeToNameMapping = new EdgeIdToNameMapping(options.netFile);
		}

		for (int i = 0; i < options.iterations; ++i) {
			traci.nextSimStep();

			Map<String, Vehicle> vehicles = traci.getVehicleRepository().getAll();

			log.debug("At step {}/{} with {} vehicles", i, options.iterations - 1, vehicles.size());

			for (Vehicle vehicle : vehicles.values()) {
				int currentSimTime = traci.getCurrentSimTime();

				String id = vehicle.getID();
				String type = vehicle.getType();

				double x = vehicle.getPosition().getX();
				double y = vehicle.getPosition().getY();
				double speed = vehicle.getSpeed();
				double fuelConsumption = vehicle.getFuelConsumption();
				double co2Emission = vehicle.getCo2Emission();

				double coEmission = vehicle.getCoEmission();
				double hcEmission = vehicle.getHcEmission();
				double noiseEmission = vehicle.getNoiseEmission();
				double noxEmission = vehicle.getNoxEmission();
				double pmxEmission = vehicle.getPmxEmission();

				// Convert from SUMO coordinates to lat/long
				Point2D position = new Point2D.Double(x, y);
				positionConversion.setObsolete();
				positionConversion.setPositionToConvert(position, true);
				Point2D point2d = positionConversion.get();

				// Determine current edge name and id
				Edge currentEdge = vehicle.getCurrentEdge();
				String edgeName = (edgeToNameMapping != null) ? edgeToNameMapping.getNameForEdgeId(currentEdge.getID()) : null;
				if (edgeName == null)
					edgeName = "unknown";

				// Write values to CSV
				if (csvPrinter != null) {
					csvPrinter.printRecord(currentSimTime, id, type, point2d.getY(), point2d.getX(), currentEdge.getID(), edgeName, speed,
							fuelConsumption, co2Emission, coEmission, hcEmission, noiseEmission, noxEmission, pmxEmission);
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

	}

}
