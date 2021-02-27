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

	int closePort(HANDLE m_hSerial);

	int readPort(HANDLE m_hSerial, int m_characters);

	std::string readLine(HANDLE m_hSerial);

}
