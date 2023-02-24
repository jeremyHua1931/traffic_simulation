/**
 * Twitter and Yammer Stream Mining with Esper Demo. This project is open-source under the terms of the GPL license. It was created and is
 * maintained by Dennis Pfisterer.
 */
package de.farberg.traci.util;

import org.slf4j.bridge.SLF4JBridgeHandler;

import de.uniluebeck.itm.util.logging.Logging;

public class LoggingUtil {

	public static void setupLogging() {
		// Optionally remove existing handlers attached to j.u.l root logger
		SLF4JBridgeHandler.removeHandlersForRootLogger(); // (since SLF4J 1.6.5)

		// add SLF4JBridgeHandler to j.u.l's root logger, should be done once during
		// the initialization phase of your application
		SLF4JBridgeHandler.install();

		Logging.setLoggingDefaults();
	}

}
