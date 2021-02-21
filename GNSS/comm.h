#pragma once

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

/* 
*		Handling USB-connection to receiver
*		At the moment only for Windows, support for Linux to come
**/



namespace gnss{

	HANDLE openPort(LPCWSTR portName); // TODO Add other parameters to call, such as parity, speed, bits et.c.
	int closePort(HANDLE m_hSerial);

	int readPort(HANDLE m_hSerial, int m_characters);

}
