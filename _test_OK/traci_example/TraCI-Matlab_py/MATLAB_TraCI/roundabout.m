%% SUMO TraCI example
clear all
close all
format compact %smaller line spacing in Command Window
clc %clears the Command Window

javaaddpath('traci4matlab.jar')

%% Main
projectPath = [pwd filesep 'roundabout.sumocfg'];

try
system(['sumo-gui' ' -c ' projectPath ' --remote-port 8813' ' --step-length    0.1' ' --start &']);
catch err
end

%initialization
[traciVersion,sumoVersion] = traci.init()

%sets the visualization scheme
traci.gui.setSchema('View #0', 'real world');

while i < 3600*10 %10 simulation steps (car following model) per second
%this runs one simulation step
traci.simulationStep();
%this is not necessary, only slows the simulation down for better display
pause(0.5)

%gets vehicle IDs
vehicles= traci.vehicle.getIDList();

%sets how the values set by setSpeed() and slowDown() shall be treated
    for ii=1:length(vehicles)
    traci.vehicle.setSpeedMode(cell2mat(vehicles(ii)),0);
    traci.vehicle.setSpeed(cell2mat(vehicles(ii)),55);
    %get actual speed, CO2 emission, distance travelled and edge ID of cars
    disp(['Speed ', cell2mat(vehicles(ii)), ': ', num2str(traci.vehicle.getSpeed(cell2mat(vehicles(ii)))) ' m/s']);
    disp(['CO2Emission ', cell2mat(vehicles(ii)), ': ', num2str(traci.vehicle.getCO2Emission(cell2mat(vehicles(ii)))) ' mg']);
    disp(['EdgeID of veh ', cell2mat(vehicles(ii)), ': ', num2str(traci.vehicle.getRoadID(cell2mat(vehicles(ii))))]);
    disp(['Distance1 ', cell2mat(vehicles(ii)), ': ', num2str(traci.vehicle.getDistance(cell2mat(vehicles(ii)))) ' m']);
    end

i=i+1;
end

traci.close();