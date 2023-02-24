import os
import sys
import optparse
from tabnanny import check

from click import option

if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'])
    sys.path.append(tools)
else:
    sys.exit("please declare environment variable 'SUMO_HOME'")
    
from sumolib import checkBinary
import traci

def get_options():
    opt_parser = optparse.OptionParser()
    opt_parser.add_option("--nogui", action="store_true", default=False, help="run the commandline version of sumo")
    options, args = opt_parser.parse_args()
    return options

#contains TraCI control loop
def run():
    step = 0
    while traci.simulation.getMinExpectedNumber() > 0:
        traci.simulationStep()
        print(step)
        step += 1
    traci.close()
    sys.stdout.flush()
    
#main entry point
if __name__ == "__main__":
    options = get_options()
    
    #checks binary
    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else: 
        sumoBinary = checkBinary('sumo-gui')   
         
    traci.start([sumoBinary, "-c", "demo.sumocfg", "--tripinfo-output", "tripinfo.xml"])
    run()