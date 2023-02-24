
VPATH = traci_testclient;utils/traci;foreign/tcpip;utils/common;CoordinateConversion

all:Traci_Client.exe

Traci_Client.exe:tracitestclient_main.o TraCITestClient.o TraCIAPI.o socket.o storage.o SUMOTime.o LatLong-UTMconversion.o SwissGrid.o UTMConversions.o
	g++ -g -o Traci_Client.exe tracitestclient_main.o TraCITestClient.o TraCIAPI.o socket.o storage.o SUMOTime.o LatLong-UTMconversion.o SwissGrid.o UTMConversions.o -l ws2_32

tracitestclient_main.o:tracitestclient_main.cpp
	g++ -c -g $^

TraCITestClient.o:TraCITestClient.cpp
	g++ -std=c++11 -c -g $^
	
TraCIAPI.o:TraCIAPI.cpp
	g++ -c -g $^
	
socket.o:socket.cpp
	g++ -c -g $^
	
storage.o:storage.cpp
	g++ -c -g $^
	
SUMOTime.o:SUMOTime.cpp
	g++ -c -g $^
	
LatLong-UTMconversion.o:LatLong-UTMconversion.cpp
	g++ -c -g $^
	
SwissGrid.o:SwissGrid.cpp
	g++ -c -g $^
	
UTMConversions.o:UTMConversions.cpp
	g++ -c -g $^

clean:
	del Traci_Client.exe tracitestclient_main.o TraCITestClient.o socket.o storage.o SUMOTime.o TraCIAPI.o LatLong-UTMconversion.o SwissGrid.o UTMConversions.o