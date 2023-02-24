import os, sys
import time

if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:   
    sys.exit("please declare environment variable 'SUMO_HOME'")
    
import traci
import traci.constants

sumoCmd = ["sumo-gui", "-c", "roundabout.sumocfg", "--start"]
traci.start(sumoCmd)

print("Starting SUMO")
traci.gui.setSchema("View #0", "real world")
    
j = 0;

while(j<60):
    #this runs one simulation step
    time.sleep(0.5);
    traci.simulationStep();
    
    vehicles=traci.vehicle.getIDList();
    if (j%10)==0: #every 10 sec....

        for i in range(0,len(vehicles)): 
            #print(len(vehicles))
            print(vehicles[i])
            traci.vehicle.setSpeedMode(vehicles[i],0)
            #sets the speed of vehicles to 15 (m/s)
            traci.vehicle.setSpeed(vehicles[i],15)
            #get actual speed, emission, edge ID and total distance travelled of vehicles
            print("Speed ", vehicles[i], ": ",traci.vehicle.getSpeed(vehicles[i]), " m/s")
            print("CO2Emission ", vehicles[i], ": ", traci.vehicle.getCO2Emission(vehicles[i]), " mg/s")
            print("EdgeID of veh ", vehicles[i], ": ", traci.vehicle.getRoadID(vehicles[i]))
            print('Distance ', vehicles[i], ": ", traci.vehicle.getDistance(vehicles[i]), " m")
        
    j = j+1;
    
#get network parameters
IDsOfEdges=traci.edge.getIDList();
print("IDs of the edges:", IDsOfEdges)
IDsOfJunctions=traci.junction.getIDList();
print("IDs of junctions:", IDsOfJunctions)

traci.close()