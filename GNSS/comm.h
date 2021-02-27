#pragma once

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<string>

/* 
*		Handling USB-connection to receiver
*		At the moment only for Windows, support for Linux to come
**/



namespace gnss{

	HANDLE openPort(LPCWSTR portName); // TODO Add other parameters to call, such as parity, data bits, stop bits, baud rates 

	int closePort(HANDLE hSerial);

	//int readPort(HANDLE hSerial, int characters);

	std::string readLineCrLf(HANDLE hSerial);

}
