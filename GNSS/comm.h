#pragma once
#define WIN32_LEAN_AND_MEAN



/* 
*		Handling USB-connection to receiver
*		At the moment only for Windows, support for Linux to come
**/

#include<Windows.h>

namespace gnss{

	HANDLE initGps();
	int closeGps(HANDLE m_hSerial);
	int readGps(HANDLE m_hSerial, int m_characters);

}
