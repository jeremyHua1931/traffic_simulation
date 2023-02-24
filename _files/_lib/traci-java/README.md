# sumo-traci-demo
This program uses [Traci4J](https://github.com/egueli/TraCI4J) to connect to a [Sumo](http://sumo.dlr.de/wiki/Main_Page) simulator instance and to control it.

## Getting started

* Import to your IDE as a Maven project (in Eclipse use Import... | Maven | Existing Maven project"). 
* Run one of the examples (e.g., de.farberg.traci.tracing.TraciSimpleTraceout) to get a help text that lists the required command line options.

Get help for TraciSimpleTraceout:
```
mvn exec:java -Dexec.mainClass="de.farberg.traci.tracing.TraciSimpleTraceout" -Dexec.args="--help"
```

Generate some sample trace of car positions: 
```
mvn exec:java -Dexec.mainClass="de.farberg.traci.tracing.TraciSimpleTraceout" -Dexec.args="-a 192.168.99.100 -p 1234 -o sim-data-out.csv -v -n 500"
```

This requires that an instance of Sumo is running and listening on 192.168.99.100:1234. Instead of installing Sumo on your local machine, you can use Docker:
* Obtain the IP adress of your Docker instance (e.g., using ```docker-machine ip default```)
* Start Sumo (follow the instructions [here](https://github.com/pfisterer/sumo-docker)).