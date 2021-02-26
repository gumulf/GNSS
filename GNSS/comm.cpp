// TODO Fix error handling, cast error?

// Note: Mostly from this document Windows Serial Port Programming, Robertson Bayer, March 30, 2008
// https://ds.opdenbrouw.nl/micprg/pdf/serial-win.pdf

#include "comm.h"
#include<iostream>


namespace gnss{

	HANDLE openPort(LPCWSTR portName){
		HANDLE hSerial;
		LPCWSTR port{portName};

		hSerial = CreateFile(port,
									GENERIC_READ,
									0,
									0,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0);

		if(hSerial == INVALID_HANDLE_VALUE){
			if(GetLastError() == ERROR_FILE_NOT_FOUND){
				std::cerr << "The requested port could not be opended!\n" << std::endl;
			} else{
				std::cerr << "Some kind of error happened opening file!\n" << std::endl;
			}
			CloseHandle(hSerial);
			return INVALID_HANDLE_VALUE;
		}

		DCB dcbSerialParams{0};

		dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

		if(!GetCommState(hSerial, &dcbSerialParams)){
			std::cerr << "Error getting state!\n" << std::endl;
			CloseHandle(hSerial);
			return INVALID_HANDLE_VALUE;
		}

		dcbSerialParams.BaudRate = CBR_4800;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.StopBits = ONESTOPBIT;
		dcbSerialParams.Parity = NOPARITY;

		int count{0};
		while(!SetCommState(hSerial, &dcbSerialParams) && count < 5){
			std::cerr << "Error setting state! Retrying!\n" << std::endl;
			++count;
			Sleep(1000);
			dcbSerialParams.BaudRate = CBR_4800;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
		}

		if(count >= 5){
			std::cerr << "Error setting state! Terminating!\n";
			CloseHandle(hSerial);
			return INVALID_HANDLE_VALUE;
		}


		COMMTIMEOUTS timeouts{0};

		timeouts.ReadIntervalTimeout = 50;
		timeouts.ReadTotalTimeoutConstant = 50;
		timeouts.ReadTotalTimeoutMultiplier = 10;
		timeouts.WriteTotalTimeoutConstant = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;

		if(!SetCommTimeouts(hSerial, &timeouts)){
			std::cerr << "Error setting timeouts!" << std::endl;
			CloseHandle(hSerial);
			return INVALID_HANDLE_VALUE;
		}

		return hSerial;
	}

	// TODO Change to return a string instead
	// TODO Remove std::cout
	int readPort(HANDLE m_hSerial, int m_characters){


		int const buffLen{1};
		//char szBuff[buffLen + 1]{};
		char szBuff{0};
		//ZeroMemory(szBuff, buffLen + 1);
		DWORD dwBytesRead{0};

		for(int i{0}; i < m_characters; ++i){
			szBuff = 0;
			do{
				if(!ReadFile(m_hSerial, &szBuff, buffLen, &dwBytesRead, NULL)){
					std::cerr << "\nError reading: " << GetLastError << std::endl;
					CloseHandle(m_hSerial);
					return -1;
				}
			} while(dwBytesRead <= 0);

			std::cout << szBuff;

		}

		std::cout << std::endl;

		return 1;

	}

	int closePort(HANDLE m_hSerial){
		CloseHandle(m_hSerial);
		return 1;
	}

}