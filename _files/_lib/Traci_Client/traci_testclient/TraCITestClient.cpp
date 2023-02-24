/****************************************************************************/
/// @file    TraCITestClient.cpp
/// @author  Friedemann Wesner
/// @author  Daniel Krajzewicz
/// @author  Laura Bieker
/// @author  Axel Wegener
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    2008/04/07
/// @version $Id$
///
/// A test execution class
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2008-2016 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include "../config.h"
#endif

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <string>

#define BUILD_TCPIP
#include "../foreign/tcpip/storage.h"
#include "../foreign/tcpip/socket.h"

#include "../traci-server/TraCIConstants.h"
#include "../utils/common/SUMOTime.h"
#include "../utils/common/ToString.h"
#include "TraCITestClient.h"
#include "../CoordinateConversion/LatLong-UTMconversion.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================
TraCITestClient::TraCITestClient(std::string outputFileName)
    : outputFileName(outputFileName), answerLog("") {
    answerLog.setf(std::ios::fixed , std::ios::floatfield); // use decimal format
    answerLog.setf(std::ios::showpoint); // print decimal point
    answerLog << std::setprecision(2);
}

TraCITestClient* TraCITestClient::getInstance()
{
	static TraCITestClient* Object = new TraCITestClient;
	return Object;
} 

TraCITestClient::~TraCITestClient() {
    writeResult();
}


void TraCITestClient::V2X_run()
{
    //Connect to the SUMO Server
	try {
            TraCIAPI::connect(host, port);
        } catch (tcpip::SocketException& e) {
            std::stringstream msg;
            msg << "#Error while connecting: " << e.what();
            errorMsg(msg);
            std::cout<<"Connection Not Established with the Server"<<std::endl;
            //return false;
        }

        Position PObj;

        subscribedVehicleData Obj;

        std::string  tlsId = "cluster_15159499_18038479_21113262_347787163_8851291";

        TraCITestClient *client = TraCITestClient::getInstance();
        //TraCIAPI::TraCIScopeWrapper TraCIScopeWrapper_Obj(TraCIAPI_Obj);

static long long int iteration = 0;
	while(1)//make it a task and it can be forever, no problem
	{
		//Run the Loop
		std::string time = std::to_string(iteration);
		//commandSimulationStep(string2time(time));

		V2X_simulationEngine(time);

		iteration++;

		int simulation_time = V2X_getSimulationTime("veh1");

		//Induce your command

		//Set

		//Get
		//std::cout<<V2X_getSpeed("veh1")<<std::endl;
		//std::cout<<V2X_getSpeed("veh2")<<std::endl;

//		PObj = V2X_getPosition("veh1");
//		std::cout<<PObj.x<<"\t";
//		std::cout<<PObj.x<<std::endl;

//		if(iteration == 20)
//		V2X_setLane("veh1","Road2");

//		if(iteration == 10)
//		V2X_stopVehicle("veh1","Road1",50,0,30);
//
//		if(iteration == 100)
//		V2X_resumeVehicle("veh1");

//		if(simulation_time == 30)
//		{	V2X_slowDown("veh1",1.378,5);
//			//V2X_changeSpeed("veh1",3);
//		}
//		if(simulation_time == 35)
//		{
//			V2X_slowDown("veh1",2.378,5000);
//		}
//
//		if(simulation_time == 5)
//		{
//			V2X_disableAutoLaneChange("veh1");
//			V2X_setSpeedMode("veh1");
//			//std::cout<<"Auto change Disabeled"<<std::endl;
//		}

		if(simulation_time == 8)
		{
			V2X_subscribeVehicleData("veh1","5","500");
			V2X_subscribeVehicleData("veh2","5","500");

		}
//		if(simulation_time == 15)
//		{
//			V2X_getsubscribedVehicleData("veh1");
//		}
//		std::cout<<"---------------------------------------------------------------"<<std::endl;
//		if(simulation_time == 18)
//		{
//			V2X_getsubscribedVehicleData("veh2");
//		}

		if(simulation_time >= 12)//Start retrieving data only after issuing V2X_subscribeVehicleData()
		{
		Obj = V2X_getsubscribedVehicleData(); //this function fills the data in vector so utilize the vector to retrieve data
//		std::cout<<"----------------Speed:"<<Obj.speed<<std::endl;
//		std::cout<<"----------------Angle:"<<Obj.angle<<std::endl;
//		std::cout<<"----------------Lane Position:"<<Obj.lanePosition<<std::endl;
//		std::cout<<"----------------Position:"<<Obj.pos_data.x<<"\t"<<Obj.pos_data.y<<std::endl;

		std::vector<subscribedVehicleData>::iterator it;

		for(it = subscribed_Vehicle_Data_Information.begin();it!=subscribed_Vehicle_Data_Information.end();it++)
		{
			//these cout make code evidently slow, so during deployment do not use these
//			std::cout<<"Vehicle Name:"<<(*it).vehicleName<<std::endl;
//			std::cout<<"Speed:"<<(*it).speed<<std::endl;
//			std::cout<<"Angle:"<<(*it).angle<<std::endl;
//			std::cout<<"Lane Position:"<<(*it).lanePosition<<std::endl;
//			std::cout<<"Position:"<<(*it).pos_data.x<<"\t"<<(*it).pos_data.y<<std::endl;

			//std::cout<<std::endl;
		}

//		std::cout<<V2X_getVehicleID("veh1");

		PObj = V2X_getPosition("veh1");
//			std::cout<<PObj.x<<"\t";
//			std::cout<<PObj.y<<std::endl;

		//probably clear the vector here before it's get too heavy
		}

		if(simulation_time >= 35)// Compute this on receipt of Msg from RSU
		{
			//int switch_time = TraCIAPI::TrafficLightScope::getNextSwitch(tlsId);
			//int switch_time = client->TraCIAPI_Obj.trafficlights.getNextSwitch(tlsId);

			std::string traffic_state = TraCIAPI::trafficlights.getRedYellowGreenState(tlsId);//to get current traffic state
			//std::cout<<traffic_state<<std::endl;
			std::string signal_state = traffic_state.substr(15,1);
			//std::cout<<signal_state<<std::endl;

			if(signal_state != "g")
			{
				int switch_time = TraCIAPI::trafficlights.getNextSwitch(tlsId);
				switch_time = switch_time - (simulation_time*1000);
				std::cout<<"------------>"<<switch_time<<std::endl;
			}
		}




		Sleep(100);


	}




        //Close the required stuff
        commandClose();//closes the storage
        close();//closes the socket

	//return 0;
}


void TraCITestClient::V2X_simulationEngine(std::string time)
{
    send_commandSimulationStep(string2time(time));
    answerLog << std::endl << "-> Command sent: <SimulationStep2>:" << std::endl;
    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, CMD_SIMSTEP2, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
        //validateSimulationStep2(inMsg);
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }

}

double TraCITestClient::V2X_getSpeed(std::string vehicleID)
{

    int domID=0xa4;
    int varID=0x40;
    double doublev =-1;
	send_commandGetVariable(domID, varID, vehicleID);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
        return -1;
    }
    check_commandGetResult(inMsg, domID, -1, false);
    // report result state
    try {
        int variableID = inMsg.readUnsignedByte();
        std::string objectID = inMsg.readString();

        int valueDataType = inMsg.readUnsignedByte();

        doublev = inMsg.readDouble();

    } catch (tcpip::SocketException& e) {
        std::stringstream msg;
        msg << "Error while receiving command: " << e.what();
        errorMsg(msg);
        return -1;
    }

	return doublev;
}

TraCITestClient::Position TraCITestClient::V2X_getPosition(std::string vehicleID)
{

    int domID=0xa4;
    int varID=0x42;

	send_commandGetVariable(domID, varID, vehicleID);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
    	pos.x = -1;
    	pos.y = -1;
        return pos;
    }
    check_commandGetResult(inMsg, domID, -1, false);
    // report result state
    try {
        int variableID = inMsg.readUnsignedByte();
        std::string objectID = inMsg.readString();

        int valueDataType = inMsg.readUnsignedByte();

        pos.x = inMsg.readDouble();
        pos.y = inMsg.readDouble();

    } catch (tcpip::SocketException& e) {
        std::stringstream msg;
        msg << "Error while receiving command: " << e.what();
        errorMsg(msg);
    	pos.x = -1;
    	pos.y = -1;
        return pos;
    }

   // TraCIPosition pos = TraCIAPI::VehicleScope::getPosition("veh1");

  //  Convert_XY_to_LatLong(pos.x,pos.y,pos.latitude,pos.longitude);//call to external code

  //  std::cout<<pos.latitude<<"\t"<<pos.longitude<<std::endl;

// now convert to actual Lat Long

//xxx

//    std::cout<<pos.x<<"/t"<<pos.y<<std::endl;
//    std::cout<<vehicleID<<std::endl;
//
//        tcpip::Storage* tmp = new tcpip::Storage;
//    	//tcpip::Storage tmp;
//
//    	tmp->writeByte(TYPE_COMPOUND);
//        tmp->writeInt(2);
//
//        tmp->writeByte(POSITION_2D);//POSITION_2D TYPE_UBYTE
////        tmp.writeUnsignedByte(TYPE_DOUBLE);
//        tmp->writeDouble(pos.x);
////        tmp.writeUnsignedByte(TYPE_DOUBLE);
//       // tmp.writeUnsignedByte(POSITION_2D);
//        tmp->writeDouble(pos.y);
//        tmp->writeByte(TYPE_UBYTE);
//
//        tmp->writeUnsignedByte(POSITION_LON_LAT);
//
//
//
//	int domIDX=0x82;
//    int varIDX=0x58;
//
//	send_commandGetVariable(domIDX, varIDX, vehicleID,tmp);
//	//send_commandSetValue(domIDX, varIDX, vehicleID, tmp);
//

    tcpip::Storage* tmp = new tcpip::Storage;
    tmp->writeByte(TYPE_COMPOUND);
    tmp->writeInt(2);
    tmp->writeByte(POSITION_2D);
    tmp->writeDouble(pos.x);
    tmp->writeDouble(pos.y);
    tmp->writeByte(TYPE_UBYTE);
    tmp->writeUnsignedByte(POSITION_LON_LAT);
    send_commandGetVariable(CMD_GET_SIM_VARIABLE, POSITION_CONVERSION, "",tmp);


    tcpip::Storage inMsgX;
    try {
        std::string acknowledgement;
        check_resultState(inMsgX, CMD_GET_SIM_VARIABLE, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
        return pos;
    }
    check_commandGetResult(inMsgX, CMD_GET_SIM_VARIABLE, -1, false);
    // report result state
    try {

        int variableID = inMsgX.readUnsignedByte();
        std::string objectID = inMsgX.readString();

        int valueDataType = inMsgX.readUnsignedByte();

        //return inMsg.readString();
       // return objectID;
        pos.longitude = inMsgX.readDouble();
        pos.latitude = inMsgX.readDouble();

       // std::cout<<pos.x<<"\t"<<pos.y<<std::endl;

    } catch (tcpip::SocketException& e) {
        std::stringstream msg;
        msg << "Error while receiving command: " << e.what();
        errorMsg(msg);
        return pos;
    }





    return pos;
}


void TraCITestClient::V2X_setLane(std::string vehicleID,std::string roadName)
{
    int domID=0xc4;
    int varID=0x31;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_STRING);
    tmp.writeString(roadName);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }
}

void TraCITestClient::V2X_stopVehicle(std::string vehicleID,std::string roadName,double positionOnLane,int laneID,int stopDuration)
{
    int domID=0xc4;
    int varID=0x12;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_COMPOUND);
    tmp.writeInt(5);

    tmp.writeUnsignedByte(TYPE_STRING);
    tmp.writeString(roadName);

    tmp.writeUnsignedByte(TYPE_DOUBLE);
    tmp.writeDouble(positionOnLane);

    tmp.writeUnsignedByte(TYPE_BYTE);
    tmp.writeByte(laneID);

    tmp.writeUnsignedByte(TYPE_INTEGER);
    tmp.writeInt(stopDuration);

    tmp.writeUnsignedByte(TYPE_BYTE);
    tmp.writeByte(2);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }
}


void TraCITestClient::V2X_resumeVehicle(std::string vehicleID)
{
    int domID=0xc4;
    int varID=0x19;

    tcpip::Storage tmp;

    tmp.writeUnsignedByte(TYPE_COMPOUND);
    tmp.writeInt(0);

    send_commandSetValue(domID, varID, vehicleID, tmp);
}

void TraCITestClient::V2X_slowDown(std::string vehicleID,double speed,int effectTime)
{
		int domID=0xc4;
	    int varID=0x14;

	    tcpip::Storage inMsg, tmp;

	    tmp.writeUnsignedByte(TYPE_COMPOUND);
	    tmp.writeInt(2);

	    tmp.writeUnsignedByte(TYPE_DOUBLE);
	    tmp.writeDouble(speed);

	    tmp.writeUnsignedByte(TYPE_INTEGER);
	    tmp.writeInt(effectTime);

	    send_commandSetValue(domID, varID, vehicleID, tmp);

	    try {
	        std::string acknowledgement;
	        check_resultState(inMsg, domID, false, &acknowledgement);
	        answerLog << acknowledgement << std::endl;
	    } catch (tcpip::SocketException& e) {
	        answerLog << e.what() << std::endl;
	    }
}


void TraCITestClient::V2X_disableAutoLaneChange(std::string vehicleID)
{
	int domID=0xc4;
    int varID=0xb6;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_INTEGER);
    tmp.writeInt(0);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }
}

void TraCITestClient::V2X_changeSpeed(std::string vehicleID,double speed)
{
	int domID=0xc4;
    int varID=0x40;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_DOUBLE);
    tmp.writeDouble(speed);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }

}

void TraCITestClient::V2X_setSpeedMode(std::string vehicleID)
{
	int domID=0xc4;
    int varID=0xb3;

    tcpip::Storage inMsg, tmp;

    tmp.writeUnsignedByte(TYPE_INTEGER);
    tmp.writeInt(0);

    send_commandSetValue(domID, varID, vehicleID, tmp);

    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }

}


int TraCITestClient::V2X_getSimulationTime(std::string vehicleID)
{
	int domID=0xab;
    int varID=0x70;
    int time = -1;

	send_commandGetVariable(domID, varID, vehicleID);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
        return -1;
    }
    check_commandGetResult(inMsg, domID, -1, false);
    // report result state
    try {

        int variableID = inMsg.readUnsignedByte();
        std::string objectID = inMsg.readString();

        int valueDataType = inMsg.readUnsignedByte();

    	time = inMsg.readInt();

    } catch (tcpip::SocketException& e) {
        std::stringstream msg;
        msg << "Error while receiving command: " << e.what();
        errorMsg(msg);
        return -1;
    }

	return (time/1000);
}

void TraCITestClient::V2X_subscribeVehicleData(std::string vehicleID,std::string beginTime,std::string endTime)
{
	int domID = 0xd4;

	const int arraySize = 4; //Number of commands you are going to give

	const int speed = 0x40;
	const int angle = 0x43;
	const int lanePosition = 0x56;
	const int position = 0x42;


	int vehicleData[arraySize] = {speed,angle,lanePosition,position};	//add your required command here
	//Important: The order in which these commands are assigned the same order should be followed to retrieve their value in "V2X_getsubscribedVehicleData" function

    std::vector<int> commands;

    for(int i=0;i<(sizeof(vehicleData)/arraySize);i++)
    {
    	commands.push_back(vehicleData[i]);
    }


	send_commandSubscribeObjectVariable(domID, vehicleID, string2time(beginTime), string2time(endTime), commands);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
      //  validateSubscription(inMsg);
        V2X_validateSubscription(inMsg);
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }

}

TraCITestClient::subscribedVehicleData TraCITestClient::V2X_getsubscribedVehicleData()
{

	tcpip::Storage inMsg;
    std::string acknowledgement;
    std::string vehicleID;

    vehicleID = V2X_getVehicleID("veh1");

    int i_time = V2X_getSimulationTime(vehicleID);
    std::string s_time = std::to_string(i_time);
    send_commandSimulationStep(string2time(s_time));

    check_resultState(inMsg, CMD_SIMSTEP2, false, &acknowledgement);

    int noSubscriptions = inMsg.readInt();
//    std::cout<<"No. of vehicles subscribed:"<<noSubscriptions<<std::endl;


    if(noSubscriptions>0)
    {
        int iteration = 0;
        do
        {
        	subscribed_Vehicle_Data = {0};

        	int length = inMsg.readUnsignedByte();
            if (length == 0) {
                length = inMsg.readInt();
            }

//            std::cout<<"Length of data:"<<length<<std::endl;

            int cmdId = inMsg.readUnsignedByte();
//            std::cout<<"Command ID:"<<cmdId<<std::endl;
            if (cmdId >= RESPONSE_SUBSCRIBE_INDUCTIONLOOP_VARIABLE && cmdId <= RESPONSE_SUBSCRIBE_GUI_VARIABLE) {
               // answerLog << "  CommandID=" << cmdId;
                //answerLog << "  ObjectID=" << inMsg.readString();

//                std::cout<<"Object ID:"<<inMsg.readString()<<std::endl;


            	subscribed_Vehicle_Data.vehicleName = inMsg.readString();


                int varNo = inMsg.readUnsignedByte();

                //answerLog << "  #variables=" << varNo << std::endl;
//                std::cout<<"No. of variables subscribed:"<<varNo<<std::endl;
    //            for (int i = 0; i < varNo; ++i) {
    //                answerLog << "      VariableID=" << inMsg.readUnsignedByte();
    //                bool ok = inMsg.readUnsignedByte() == RTYPE_OK;
    //                answerLog << "      ok=" << ok;
    //                int valueDataType = inMsg.readUnsignedByte();
    //                answerLog << " valueDataType=" << valueDataType;
    //                readAndReportTypeDependent(inMsg, valueDataType);

                bool ok;int valueDataType;

                //Important:: The commands are called in the same sequence in which they were set in "V2X_subscribeVehicleData" function

               //speed
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

                inMsg.readUnsignedByte();

                ok = inMsg.readUnsignedByte() == RTYPE_OK;

                valueDataType = inMsg.readUnsignedByte();

                subscribed_Vehicle_Data.speed = inMsg.readDouble();



                //angle
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

                inMsg.readUnsignedByte();

                ok = inMsg.readUnsignedByte() == RTYPE_OK;

                valueDataType = inMsg.readUnsignedByte();

                subscribed_Vehicle_Data.angle = inMsg.readDouble();

                //lane position
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

                inMsg.readUnsignedByte();

                ok = inMsg.readUnsignedByte() == RTYPE_OK;

                valueDataType = inMsg.readUnsignedByte();

                subscribed_Vehicle_Data.lanePosition = inMsg.readDouble();

                //position
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

                inMsg.readUnsignedByte();

                ok = inMsg.readUnsignedByte() == RTYPE_OK;

                valueDataType = inMsg.readUnsignedByte();

                subscribed_Vehicle_Data.pos_data.x = inMsg.readDouble();
                subscribed_Vehicle_Data.pos_data.y = inMsg.readDouble();
                //here we will get XY coordinates only, we have explicitly call the conversion function to convert it to lat long

                }


            subscribed_Vehicle_Data_Information.push_back(subscribed_Vehicle_Data);// So entire retrieved data is in this vector, you can utilize it in your code

            iteration++;

        }while(iteration<noSubscriptions);

    }
    else
    {
    	std::cout<<"Chill-Vehicles are gone"<<std::endl;
    	subscribed_Vehicle_Data_Information.clear();

    }

    return subscribed_Vehicle_Data;

}


std::string TraCITestClient::V2X_getVehicleID(std::string vehicleID)
{
	int domID=0xab;
    int varID=0x72;

	send_commandGetVariable(domID, varID, vehicleID);

    tcpip::Storage inMsg;
    try {
        std::string acknowledgement;
        check_resultState(inMsg, domID, false, &acknowledgement);

    } catch (tcpip::SocketException& e) {
        return "NULL";
    }
    check_commandGetResult(inMsg, domID, -1, false);
    // report result state
    try {

        int variableID = inMsg.readUnsignedByte();
        std::string objectID = inMsg.readString();

        int valueDataType = inMsg.readUnsignedByte();

        //return inMsg.readString();
        return objectID;

    } catch (tcpip::SocketException& e) {
        std::stringstream msg;
        msg << "Error while receiving command: " << e.what();
        errorMsg(msg);
        return "NULL";
    }

}


// ---------- Commands handling
//void
//TraCITestClient::commandSimulationStep(SUMOTime time) {
//    send_commandSimulationStep(time);
//    answerLog << std::endl << "-> Command sent: <SimulationStep2>:" << std::endl;
//    tcpip::Storage inMsg;
//    try {
//        std::string acknowledgement;
//        check_resultState(inMsg, CMD_SIMSTEP2, false, &acknowledgement);
//        answerLog << acknowledgement << std::endl;
//        //validateSimulationStep2(inMsg);
//    } catch (tcpip::SocketException& e) {
//        answerLog << e.what() << std::endl;
//    }
//}


void
TraCITestClient::commandClose() {
    send_commandClose();
    answerLog << std::endl << "-> Command sent: <Close>:" << std::endl;
    try {
        tcpip::Storage inMsg;
        std::string acknowledgement;
        check_resultState(inMsg, CMD_CLOSE, false, &acknowledgement);
        answerLog << acknowledgement << std::endl;
    } catch (tcpip::SocketException& e) {
        answerLog << e.what() << std::endl;
    }
}


//void
//TraCITestClient::commandGetVariable(int domID, int varID, const std::string& objID, tcpip::Storage* addData) {
//    send_commandGetVariable(domID, varID, objID, addData);
//    answerLog << std::endl << "-> Command sent: <GetVariable>:" << std::endl
//              << "  domID=" << domID << " varID=" << varID
//              << " objID=" << objID << std::endl;
//    tcpip::Storage inMsg;
//    try {
//        std::string acknowledgement;
//        check_resultState(inMsg, domID, false, &acknowledgement);
//        answerLog << acknowledgement << std::endl;
//    } catch (tcpip::SocketException& e) {
//        answerLog << e.what() << std::endl;
//        return;
//    }
//    check_commandGetResult(inMsg, domID, -1, false);
//    // report result state
//    try {
//        int variableID = inMsg.readUnsignedByte();
//        std::string objectID = inMsg.readString();
//        answerLog <<  "  CommandID=" << (domID + 0x10) << "  VariableID=" << variableID << "  ObjectID=" << objectID;
//        int valueDataType = inMsg.readUnsignedByte();
//        answerLog << " valueDataType=" << valueDataType;
//        readAndReportTypeDependent(inMsg, valueDataType);
//    } catch (tcpip::SocketException& e) {
//        std::stringstream msg;
//        msg << "Error while receiving command: " << e.what();
//        errorMsg(msg);
//        return;
//    }
//}


//void
//TraCITestClient::commandSetValue(int domID, int varID, const std::string& objID, std::ifstream& defFile) {
//    std::stringstream msg;
//    tcpip::Storage inMsg, tmp;
//    setValueTypeDependant(tmp, defFile, msg);
//    std::string msgS = msg.str();
//    if (msgS != "") {
//        errorMsg(msg);
//    }
//    send_commandSetValue(domID, varID, objID, tmp);
//    answerLog << std::endl << "-> Command sent: <SetValue>:" << std::endl
//              << "  domID=" << domID << " varID=" << varID
//              << " objID=" << objID << std::endl;
//    try {
//        std::string acknowledgement;
//        check_resultState(inMsg, domID, false, &acknowledgement);
//        answerLog << acknowledgement << std::endl;
//    } catch (tcpip::SocketException& e) {
//        answerLog << e.what() << std::endl;
//    }
//}


//void
//TraCITestClient::commandSubscribeObjectVariable(int domID, const std::string& objID, SUMOTime beginTime, SUMOTime endTime, int varNo, std::ifstream& defFile) {
//    std::vector<int> vars;
//    for (int i = 0; i < varNo; ++i) {
//        int var;
//        defFile >> var;
//        // variable id
//        vars.push_back(var);
//    }
//    send_commandSubscribeObjectVariable(domID, objID, beginTime, endTime, vars);
//    answerLog << std::endl << "-> Command sent: <SubscribeVariable>:" << std::endl
//              << "  domID=" << domID << " objID=" << objID << " with " << varNo << " variables" << std::endl;
//    tcpip::Storage inMsg;
//    try {
//        std::string acknowledgement;
//        check_resultState(inMsg, domID, false, &acknowledgement);
//        answerLog << acknowledgement << std::endl;
//        validateSubscription(inMsg);
//    } catch (tcpip::SocketException& e) {
//        answerLog << e.what() << std::endl;
//    }
//}

//
//void
//TraCITestClient::commandSubscribeContextVariable(int domID, const std::string& objID, SUMOTime beginTime, SUMOTime endTime,
//        int domain, SUMOReal range, int varNo, std::ifstream& defFile) {
//    std::vector<int> vars;
//    for (int i = 0; i < varNo; ++i) {
//        int var;
//        defFile >> var;
//        // variable id
//        vars.push_back(var);
//    }
//    send_commandSubscribeObjectContext(domID, objID, beginTime, endTime, domain, range, vars);
//    answerLog << std::endl << "-> Command sent: <SubscribeContext>:" << std::endl
//              << "  domID=" << domID << " objID=" << objID << " domain=" << domain << " range=" << range
//              << " with " << varNo << " variables" << std::endl;
//    tcpip::Storage inMsg;
//    try {
//        std::string acknowledgement;
//        check_resultState(inMsg, domID, false, &acknowledgement);
//        answerLog << acknowledgement << std::endl;
//        validateSubscription(inMsg);
//    } catch (tcpip::SocketException& e) {
//        answerLog << e.what() << std::endl;
//    }
//}


// ---------- Report helper
void
TraCITestClient::writeResult() {
    time_t seconds;
    tm* locTime;
    std::ofstream outFile(outputFileName.c_str());
    if (!outFile) {
        std::cerr << "Unable to write result file" << std::endl;
    }
    time(&seconds);
    locTime = localtime(&seconds);
    outFile << "TraCITestClient output file. Date: " << asctime(locTime) << std::endl;
    outFile << answerLog.str();
    outFile.close();
}


void
TraCITestClient::errorMsg(std::stringstream& msg) {
    std::cerr << msg.str() << std::endl;
    answerLog << "----" << std::endl << msg.str() << std::endl;
}






//bool
//TraCITestClient::validateSimulationStep2(tcpip::Storage& inMsg) {
//    try {
//        int noSubscriptions = inMsg.readInt();
//		//std::cout<<"No. of subscription"<<noSubscriptions<<std::endl;
//        for (int s = 0; s < noSubscriptions; ++s) {
//
//            if (!validateSubscription(inMsg)) {
//                return false;
//            }
//        }
//    } catch (std::invalid_argument& e) {
//        answerLog << "#Error while reading message:" << e.what() << std::endl;
//        return false;
//    }
//    return true;
//}


//bool
//TraCITestClient::validateSubscription(tcpip::Storage& inMsg) {
//    try {
//        int length = inMsg.readUnsignedByte();
//        if (length == 0) {
//            length = inMsg.readInt();
//        }
//        int cmdId = inMsg.readUnsignedByte();
//        if (cmdId >= RESPONSE_SUBSCRIBE_INDUCTIONLOOP_VARIABLE && cmdId <= RESPONSE_SUBSCRIBE_GUI_VARIABLE) {
//            answerLog << "  CommandID=" << cmdId;
//            answerLog << "  ObjectID=" << inMsg.readString();
//            int varNo = inMsg.readUnsignedByte();
//            answerLog << "  #variables=" << varNo << std::endl;
//            for (int i = 0; i < varNo; ++i) {
//                answerLog << "      VariableID=" << inMsg.readUnsignedByte();
//                bool ok = inMsg.readUnsignedByte() == RTYPE_OK;
//                answerLog << "      ok=" << ok;
//                int valueDataType = inMsg.readUnsignedByte();
//                answerLog << " valueDataType=" << valueDataType;
//                readAndReportTypeDependent(inMsg, valueDataType);
//            }
//        } else if (cmdId >= RESPONSE_SUBSCRIBE_INDUCTIONLOOP_CONTEXT && cmdId <= RESPONSE_SUBSCRIBE_GUI_CONTEXT) {
//            answerLog << "  CommandID=" << cmdId;
//            answerLog << "  ObjectID=" << inMsg.readString();
//            answerLog << "  Domain=" << inMsg.readUnsignedByte();
//            int varNo = inMsg.readUnsignedByte();
//            answerLog << "  #variables=" << varNo << std::endl;
//            int objNo = inMsg.readInt();
//            answerLog << "  #objects=" << objNo << std::endl;
//            for (int j = 0; j < objNo; ++j) {
//                answerLog << "   ObjectID=" << inMsg.readString() << std::endl;
//                for (int i = 0; i < varNo; ++i) {
//                    answerLog << "      VariableID=" << inMsg.readUnsignedByte();
//                    bool ok = inMsg.readUnsignedByte() == RTYPE_OK;
//                    answerLog << "      ok=" << ok;
//                    int valueDataType = inMsg.readUnsignedByte();
//                    answerLog << " valueDataType=" << valueDataType;
//                    readAndReportTypeDependent(inMsg, valueDataType);
//                }
//            }
//        } else {
//            answerLog << "#Error: received response with command id: " << cmdId << " but expected a subscription response (0xe0-0xef / 0x90-0x9f)" << std::endl;
//            return false;
//        }
//    } catch (std::invalid_argument& e) {
//        answerLog << "#Error while reading message:" << e.what() << std::endl;
//        return false;
//    }
//    return true;
//}

bool
TraCITestClient::V2X_validateSubscription(tcpip::Storage& inMsg)
{

	subscribed_Vehicle_Data = {0};

	int length = inMsg.readUnsignedByte();
    if (length == 0) {
        length = inMsg.readInt();
    }

//            std::cout<<"Length of data:"<<length<<std::endl;

    int cmdId = inMsg.readUnsignedByte();
//            std::cout<<"Command ID:"<<cmdId<<std::endl;
    if (cmdId >= RESPONSE_SUBSCRIBE_INDUCTIONLOOP_VARIABLE && cmdId <= RESPONSE_SUBSCRIBE_GUI_VARIABLE) {
       // //answerLog << "  CommandID=" << cmdId;
        ////answerLog << "  ObjectID=" << inMsg.readString();

//                std::cout<<"Object ID:"<<inMsg.readString()<<std::endl;


    	subscribed_Vehicle_Data.vehicleName = inMsg.readString();


        int varNo = inMsg.readUnsignedByte();

        ////answerLog << "  #variables=" << varNo << std::endl;
//                std::cout<<"No. of variables subscribed:"<<varNo<<std::endl;
//            for (int i = 0; i < varNo; ++i) {
//                //answerLog << "      VariableID=" << inMsg.readUnsignedByte();
//                bool ok = inMsg.readUnsignedByte() == RTYPE_OK;
//                //answerLog << "      ok=" << ok;
//                int valueDataType = inMsg.readUnsignedByte();
//                //answerLog << " valueDataType=" << valueDataType;
//                readAndReportTypeDependent(inMsg, valueDataType);

        bool ok;int valueDataType;

        //Important:: The commands are called in the same sequence in which they were set in "V2X_subscribeVehicleData" function

       //speed
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

        inMsg.readUnsignedByte();

        ok = inMsg.readUnsignedByte() == RTYPE_OK;

        valueDataType = inMsg.readUnsignedByte();

        subscribed_Vehicle_Data.speed = inMsg.readDouble();



        //angle
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

        inMsg.readUnsignedByte();

        ok = inMsg.readUnsignedByte() == RTYPE_OK;

        valueDataType = inMsg.readUnsignedByte();

        subscribed_Vehicle_Data.angle = inMsg.readDouble();

        //lane position
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

        inMsg.readUnsignedByte();

        ok = inMsg.readUnsignedByte() == RTYPE_OK;

        valueDataType = inMsg.readUnsignedByte();

        subscribed_Vehicle_Data.lanePosition = inMsg.readDouble();

        //position
//                std::cout<<"Variable ID:"<<inMsg.readUnsignedByte()<<std::endl;

        inMsg.readUnsignedByte();

        ok = inMsg.readUnsignedByte() == RTYPE_OK;

        valueDataType = inMsg.readUnsignedByte();

        subscribed_Vehicle_Data.pos_data.x = inMsg.readDouble();
        subscribed_Vehicle_Data.pos_data.y = inMsg.readDouble();


        }

    std::cout<<"Vehicle Name:"<<subscribed_Vehicle_Data.vehicleName<<std::endl;
    std::cout<<"Vehicle Speed:"<<subscribed_Vehicle_Data.speed<<std::endl;
    std::cout<<"Vehicle Angle:"<<subscribed_Vehicle_Data.angle<<std::endl;
    std::cout<<"Vehicle Lane position:"<<subscribed_Vehicle_Data.lanePosition<<std::endl;
    std::cout<<"Vehicle Location:"<<subscribed_Vehicle_Data.pos_data.x<<"\t"<<subscribed_Vehicle_Data.pos_data.y<<std::endl;


   // subscribed_Vehicle_Data_Information.push_back(subscribed_Vehicle_Data);

   // iteration++;


	return true;
}







// ---------- Conversion helper
//int
//TraCITestClient::setValueTypeDependant(tcpip::Storage& into, std::ifstream& defFile, std::stringstream& msg) {
//    std::string dataTypeS;
//    defFile >> dataTypeS;
//    if (dataTypeS == "<airDist>") {
//        into.writeUnsignedByte(REQUEST_AIRDIST);
//        return 1;
//    } else if (dataTypeS == "<drivingDist>") {
//        into.writeUnsignedByte(REQUEST_DRIVINGDIST);
//        return 1;
//    } else if (dataTypeS == "<objSubscription>") {
//        int beginTime, endTime, numVars;
//        defFile >> beginTime >> endTime >> numVars;
//        into.writeInt(beginTime);
//        into.writeInt(endTime);
//        into.writeInt(numVars);
//        for (int i = 0; i < numVars; ++i) {
//            int var;
//            defFile >> var;
//            into.writeUnsignedByte(var);
//        }
//        return 4 + 4 + 4 + numVars;
//    }
//    int valI;
//    double valF;
//    if (dataTypeS == "<int>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_INTEGER);
//        into.writeInt(valI);
//        return 4 + 1;
//    } else if (dataTypeS == "<byte>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_BYTE);
//        into.writeByte(valI);
//        return 1 + 1;
//    }  else if (dataTypeS == "<ubyte>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_UBYTE);
//        into.writeUnsignedByte(valI);
//        return 1 + 1;
//    } else if (dataTypeS == "<float>") {
//        defFile >> valF;
//        into.writeUnsignedByte(TYPE_FLOAT);
//        into.writeFloat(float(valF));
//        return 4 + 1;
//    } else if (dataTypeS == "<double>") {
//        defFile >> valF;
//        into.writeUnsignedByte(TYPE_DOUBLE);
//        into.writeDouble(valF);
//        return 8 + 1;
//    } else if (dataTypeS == "<string>") {
//        std::string valueS;
//        defFile >> valueS;
//        if (valueS == "\"\"") {
//            valueS = "";
//        }
//        into.writeUnsignedByte(TYPE_STRING);
//        into.writeString(valueS);
//        return 4 + 1 + (int) valueS.length();
//    } else if (dataTypeS == "<string*>") {
//        std::vector<std::string> slValue;
//        defFile >> valI;
//        int length = 1 + 4;
//        for (int i = 0; i < valI; ++i) {
//            std::string tmp;
//            defFile >> tmp;
//            slValue.push_back(tmp);
//            length += 4 + int(tmp.length());
//        }
//        into.writeUnsignedByte(TYPE_STRINGLIST);
//        into.writeStringList(slValue);
//        return length;
//    } else if (dataTypeS == "<compound>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_COMPOUND);
//        into.writeInt(valI);
//        int length = 1 + 4;
//        for (int i = 0; i < valI; ++i) {
//            length += setValueTypeDependant(into, defFile, msg);
//        }
//        return length;
//    } else if (dataTypeS == "<color>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_COLOR);
//        into.writeUnsignedByte(valI);
//        for (int i = 0; i < 3; ++i) {
//            defFile >> valI;
//            into.writeUnsignedByte(valI);
//        }
//        return 1 + 4;
//    } else if (dataTypeS == "<position2D>") {
//        defFile >> valF;
//        into.writeUnsignedByte(POSITION_2D);
//        into.writeDouble(valF);
//        defFile >> valF;
//        into.writeDouble(valF);
//        return 1 + 8 + 8;
//    } else if (dataTypeS == "<position3D>") {
//        defFile >> valF;
//        into.writeUnsignedByte(POSITION_3D);
//        into.writeDouble(valF);
//        defFile >> valF;
//        into.writeDouble(valF);
//        defFile >> valF;
//        into.writeDouble(valF);
//        return 1 + 8 + 8 + 8;
//    } else if (dataTypeS == "<positionRoadmap>") {
//        std::string valueS;
//        defFile >> valueS;
//        into.writeUnsignedByte(POSITION_ROADMAP);
//        into.writeString(valueS);
//        int length = 1 + 8 + (int) valueS.length();
//        defFile >> valF;
//        into.writeDouble(valF);
//        defFile >> valI;
//        into.writeUnsignedByte(valI);
//        return length + 4 + 1;
//    } else if (dataTypeS == "<shape>") {
//        defFile >> valI;
//        into.writeUnsignedByte(TYPE_POLYGON);
//        into.writeUnsignedByte(valI);
//        int length = 1 + 1;
//        for (int i = 0; i < valI; ++i) {
//            double x, y;
//            defFile >> x >> y;
//            into.writeDouble(x);
//            into.writeDouble(y);
//            length += 8 + 8;
//        }
//        return length;
//    }
//    msg << "## Unknown data type: " << dataTypeS;
//    return 0;
//}


//void
//TraCITestClient::readAndReportTypeDependent(tcpip::Storage& inMsg, int valueDataType) {
//    if (valueDataType == TYPE_UBYTE) {
//        int ubyte = inMsg.readUnsignedByte();
//        answerLog << " Unsigned Byte Value: " << ubyte << std::endl;
//    } else if (valueDataType == TYPE_BYTE) {
//        int byte = inMsg.readByte();
//        answerLog << " Byte value: " << byte << std::endl;
//    } else if (valueDataType == TYPE_INTEGER) {
//        int integer = inMsg.readInt();
//        answerLog << " Int value: " << integer << std::endl;
//    } else if (valueDataType == TYPE_FLOAT) {
//        float floatv = inMsg.readFloat();
//        if (floatv < 0.1 && floatv > 0) {
//            answerLog.setf(std::ios::scientific, std::ios::floatfield);
//        }
//        answerLog << " float value: " << floatv << std::endl;
//        answerLog.setf(std::ios::fixed , std::ios::floatfield); // use decimal format
//        answerLog.setf(std::ios::showpoint); // print decimal point
//        answerLog << std::setprecision(2);
//    } else if (valueDataType == TYPE_DOUBLE) {
//        double doublev = inMsg.readDouble();
//        answerLog << " Double value: " << doublev << std::endl;
//		//std::cout<<doublev<<std::endl;
//    } else if (valueDataType == TYPE_BOUNDINGBOX) {
//        SUMOReal lowerLeftX = inMsg.readDouble();
//        SUMOReal lowerLeftY = inMsg.readDouble();
//        SUMOReal upperRightX = inMsg.readDouble();
//        SUMOReal upperRightY = inMsg.readDouble();
//        answerLog << " BoundaryBoxValue: lowerLeft x=" << lowerLeftX
//                  << " y=" << lowerLeftY << " upperRight x=" << upperRightX
//                  << " y=" << upperRightY << std::endl;
//    } else if (valueDataType == TYPE_POLYGON) {
//        int length = inMsg.readUnsignedByte();
//        answerLog << " PolygonValue: ";
//        for (int i = 0; i < length; i++) {
//            SUMOReal x = inMsg.readDouble();
//            SUMOReal y = inMsg.readDouble();
//            answerLog << "(" << x << "," << y << ") ";
//        }
//        answerLog << std::endl;
//    } else if (valueDataType == POSITION_3D) {
//        SUMOReal x = inMsg.readDouble();
//        SUMOReal y = inMsg.readDouble();
//        SUMOReal z = inMsg.readDouble();
//        answerLog << " Position3DValue: " << std::endl;
//        answerLog << " x: " << x << " y: " << y
//                  << " z: " << z << std::endl;
//    } else if (valueDataType == POSITION_ROADMAP) {
//        std::string roadId = inMsg.readString();
//        SUMOReal pos = inMsg.readDouble();
//        int laneId = inMsg.readUnsignedByte();
//        answerLog << " RoadMapPositionValue: roadId=" << roadId
//                  << " pos=" << pos
//                  << " laneId=" << laneId << std::endl;
//    } else if (valueDataType == TYPE_TLPHASELIST) {
//        int length = inMsg.readUnsignedByte();
//        answerLog << " TLPhaseListValue: length=" << length << std::endl;
//        for (int i = 0; i < length; i++) {
//            std::string pred = inMsg.readString();
//            std::string succ = inMsg.readString();
//            int phase = inMsg.readUnsignedByte();
//            answerLog << " precRoad=" << pred << " succRoad=" << succ
//                      << " phase=";
//            switch (phase) {
//                case TLPHASE_RED:
//                    answerLog << "red" << std::endl;
//                    break;
//                case TLPHASE_YELLOW:
//                    answerLog << "yellow" << std::endl;
//                    break;
//                case TLPHASE_GREEN:
//                    answerLog << "green" << std::endl;
//                    break;
//                default:
//                    answerLog << "#Error: unknown phase value" << (int)phase << std::endl;
//                    return;
//            }
//        }
//    } else if (valueDataType == TYPE_STRING) {
//        std::string s = inMsg.readString();
//        answerLog << " string value: " << s << std::endl;
//    } else if (valueDataType == TYPE_STRINGLIST) {
//        std::vector<std::string> s = inMsg.readStringList();
//        answerLog << " string list value: [ " << std::endl;
//        for (std::vector<std::string>::iterator i = s.begin(); i != s.end(); ++i) {
//            if (i != s.begin()) {
//                answerLog << ", ";
//            }
//            answerLog << '"' << *i << '"';
//        }
//        answerLog << " ]" << std::endl;
//    } else if (valueDataType == TYPE_COMPOUND) {
//        int no = inMsg.readInt();
//        answerLog << " compound value with " << no << " members: [ " << std::endl;
//        for (int i = 0; i < no; ++i) {
//            int currentValueDataType = inMsg.readUnsignedByte();
//            answerLog << " valueDataType=" << currentValueDataType;
//            readAndReportTypeDependent(inMsg, currentValueDataType);
//        }
//        answerLog << " ]" << std::endl;
//    } else if (valueDataType == POSITION_2D) {
//        SUMOReal xv = inMsg.readDouble();
//        SUMOReal yv = inMsg.readDouble();
//        answerLog << " position value: (" << xv << "," << yv << ")" << std::endl;
//    } else if (valueDataType == TYPE_COLOR) {
//        int r = inMsg.readUnsignedByte();
//        int g = inMsg.readUnsignedByte();
//        int b = inMsg.readUnsignedByte();
//        int a = inMsg.readUnsignedByte();
//        answerLog << " color value: (" << r << "," << g << "," << b << "," << a << ")" << std::endl;
//    } else {
//        answerLog << "#Error: unknown valueDataType!" << std::endl;
//    }
//}

//
//void
//TraCITestClient::testAPI() {
//    answerLog << "testAPI:\n";
//
//    answerLog << "  edge:\n";
//    answerLog << "    getIDList: " << joinToString(edge.getIDList(), " ") << "\n";
//    answerLog << "    getIDCount: " << edge.getIDCount() << "\n";
//    answerLog << "  route:\n";
//    answerLog << "    add:\n";
//    std::vector<std::string> edges;
//    edges.push_back("e_u1");
//    route.add("e_u1", edges);
//    answerLog << "    getIDList: " << joinToString(route.getIDList(), " ") << "\n";
//    answerLog << "  vehicle:\n";
//    answerLog << "    getRoadID: " << vehicle.getRoadID("0") << "\n";
//    answerLog << "    getLaneID: " << vehicle.getLaneID("0") << "\n";
//    answerLog << "    getSpeedMode: " << vehicle.getSpeedMode("0") << "\n";
//    answerLog << "    getSlope: " << vehicle.getSlope("0") << "\n";
//    TraCIColor col1;
//    col1.r = 255;
//    col1.g = 255;
//    col1.b = 0;
//    col1.a = 128;
//    vehicle.setColor("0", col1);
//    TraCIColor col2 = vehicle.getColor("0");
//    answerLog << "    getColor: " << col2.r << "r=" << col2.r << " g=" << col2.g << " b=" << col2.b << " a=" << col2.a << "\n";
//    answerLog << "    getNextTLS:\n";
//    std::vector<VehicleScope::NextTLSData> result = vehicle.getNextTLS("0");
//    for (int i = 0; i < (int)result.size(); ++i) {
//        const VehicleScope::NextTLSData& d = result[i];
//        answerLog << "      tls=" << d.id << " tlIndex=" << d.tlIndex << " dist=" << d.dist << " state=" << d.state << "\n";
//    }
//    answerLog << "    moveToXY, simStep:\n";
//    vehicle.moveToXY("0", "dummy", 0, 2231.61, 498.29, 90, 1);
//    simulationStep();
//    answerLog << "    getRoadID: " << vehicle.getRoadID("0") << "\n";
//    answerLog << "    getLaneID: " << vehicle.getLaneID("0") << "\n";
//    answerLog << "    add:\n";
//    vehicle.add("1", "e_u1");
//    simulationStep();
//    answerLog << "    getIDList: " << joinToString(vehicle.getIDList(), " ") << "\n";
//    answerLog << "    getWaitingTime: " << vehicle.getWaitingTime("0") << "\n";
//    answerLog << "    remove:\n";
//    vehicle.remove("0");
//    answerLog << "    getIDCount: " << vehicle.getIDCount() << "\n";
//
//    answerLog << "  inductionloop:\n";
//    answerLog << "    getIDList: " << joinToString(inductionloop.getIDList(), " ") << "\n";
//    answerLog << "    getVehicleData:\n";
//    std::vector<InductionLoopScope::VehicleData> result2 = inductionloop.getVehicleData("det1");
//    for (int i = 0; i < (int)result2.size(); ++i) {
//        const InductionLoopScope::VehicleData& vd = result2[i];
//        answerLog << "      veh=" << vd.id << " length=" << vd.length << " entered=" << vd.entryTime << " left=" << vd.leaveTime << " type=" << vd.typeID << "\n";
//    }
//
//    answerLog << "  simulation:\n";
//    answerLog << "    getCurrentTime: " << simulation.getCurrentTime() << "\n";
//    answerLog << "    subscribe to road and pos of vehicle '1':\n";
//    std::vector<int> vars;
//    vars.push_back(VAR_ROAD_ID);
//    vars.push_back(VAR_LANEPOSITION);
//    simulation.subscribe(CMD_SUBSCRIBE_VEHICLE_VARIABLE, "1", 0, TIME2STEPS(100), vars);
//    simulationStep();
//    answerLog << "    subscription results:\n";
//    TraCIValues result3 = simulation.getSubscriptionResults("1");
//    answerLog << "      roadID=" << result3[VAR_ROAD_ID].string << " pos=" << result3[VAR_LANEPOSITION].scalar << "\n";
//
//    answerLog << "    subscribe to vehicles around edge 'e_u1':\n";
//    std::vector<int> vars2;
//    vars2.push_back(VAR_LANEPOSITION);
//    simulation.subscribeContext(CMD_SUBSCRIBE_EDGE_CONTEXT, "e_u1", 0, TIME2STEPS(100), CMD_GET_VEHICLE_VARIABLE, 100, vars2);
//    simulationStep();
//    answerLog << "    context subscription results:\n";
//    SubscribedValues result4 = simulation.getContextSubscriptionResults("e_u1");
//    for (SubscribedValues::iterator it = result4.begin(); it != result4.end(); ++it) {
//        answerLog << "      vehicle=" << it->first << " pos=" << it->second[VAR_LANEPOSITION].scalar << "\n";
//    }
//
//    answerLog << "  person:\n";
//    answerLog << "    getIDList: " << joinToString(person.getIDList(), " ") << "\n";
//    answerLog << "    getRoadID: " << person.getRoadID("p0") << "\n";
//    answerLog << "    getTypeID: " << person.getTypeID("p0") << "\n";
//    answerLog << "    getWaitingTime: " << person.getWaitingTime("p0") << "\n";
//    answerLog << "    getNextEdge: " << person.getNextEdge("p0") << "\n";
//
//    answerLog << "  gui:\n";
//    try {
//        answerLog << "    setScheme: \n";
//        gui.setSchema("View #0", "real world");
//        answerLog << "    getScheme: " << gui.getSchema("View #0") << "\n";
//        answerLog << "    take screenshot: \n";
//        gui.screenshot("View #0", "image.png");
//    } catch (tcpip::SocketException&) {
//        answerLog << "    no support for gui commands\n";
//    }
//}
