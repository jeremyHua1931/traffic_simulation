package de.farberg.traci.util;

import java.io.File;

import org.kohsuke.args4j.Option;

public class CommandLineOptionsTemplate {

	@Option(name = "-o", aliases = { "--csv-out" }, usage = "Filename for the CSV file (optional)", required = false)
	public String csvOutFile = null;

	@Option(name = "-a", aliases = { "--traci-address" }, usage = "Traci address", required = true)
	public String traciAddress = null;

	@Option(name = "-p", aliases = { "--traci-port" }, usage = "Traci port", required = true)
	public int traciPort = -1;

	@Option(name = "--net-file", usage = "Sumo net file", required = false)
	public File netFile = null;

	@Option(name = "-n", aliases = { "--iterations" }, usage = "Number of iterations", required = false)
	public int iterations = 10;

	@Option(name = "-h", aliases = { "--help" }, usage = "This help message.", required = false)
	public boolean help = false;

	@Option(name = "-v", aliases = { "--verbose" }, usage = "Verbose (DEBUG) logging output (default: INFO).", required = false)
	public boolean verbose = false;

}