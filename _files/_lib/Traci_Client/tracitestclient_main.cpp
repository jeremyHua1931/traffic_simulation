/****************************************************************************/
/// @file    tracitestclient_main.h
/// @author  Friedemann Wesner
/// @author  Axel Wegener
/// @author  Michael Behrisch
/// @author  Daniel Krajzewicz
/// @date    2008/04/07
/// @version $Id$
///
/// Main method for TraCITestClient
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2001-2016 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


#ifdef _MSC_VER
#include <windows_config.h>
#else
#include "config.h"
#endif

#include <iostream>
#include <string>
#include <cstdlib>
#include "traci_testclient/TraCITestClient.h"

//#define CREATETHREAD


int main()
{


	TraCITestClient *client = TraCITestClient::getInstance();
	

	
#ifdef CREATETHREAD
	client->container();
#else
	client->V2X_run();
#endif

	return 0;
}

void TraCITestClient::container()
{

	
		TraCITestClient *client = TraCITestClient::getInstance();
		
	TraCITestClient::PMYDATA pDataArray;
	

	
	    pDataArray = (TraCITestClient::PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(TraCITestClient::MYDATA));

        if(pDataArray == NULL )
        {
           // If the array allocation fails, the system is out of memory
           // so there is no point in trying to print an error message.
           // Just terminate execution.
            ExitProcess(2);
        }
		
		HANDLE  hThreadArray; 
		DWORD   dwThreadIdArray;
		
		hThreadArray =  CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            //MyThreadFunction,       // thread function name
			(LPTHREAD_START_ROUTINE)TraCITestClient::myFunctionCaller,
            this,          // argument to thread function 
			0,                      // use default creation flags 
            &dwThreadIdArray);   // returns the thread identifier 
			
		if (hThreadArray == NULL) 
        {
           client->ErrorHandler(TEXT("CreateThread"));
           ExitProcess(3);
        }
		
		// Wait until thread is terminated.
		std::cout<<"Created Thread now having fun"<<std::endl;
		
		//Post your functions here
		

		WaitForSingleObject(hThreadArray,-1);
		
		std::cout<<"Thread terminated--------------------------------------------------------------------------------------------------"<<std::endl;
		// Close thread handles and free memory allocations.
		
		CloseHandle(hThreadArray);
        if(pDataArray != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray);
            pDataArray = NULL;    // Ensure address is not reused.
        }
		
		//**********************************************
}

DWORD TraCITestClient::myFunctionCaller(LPVOID param)
{
		TraCITestClient* client = static_cast<TraCITestClient*>(param);
		client->V2X_run();
}

void TraCITestClient:: ErrorHandler(LPTSTR lpszFunction) 
{ 
    // Retrieve the system error message for the last-error code.

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) * sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"), 
        lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK); 

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
