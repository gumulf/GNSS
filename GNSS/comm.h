#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <stdexcept>
#include "ctrlhandler.h"

/* 
*		Handling USB-connection to receiver
*		At the moment only for Windows, support for Linux to come
**/



namespace gnss{

	HANDLE openPort(LPCWSTR port_name); // TODO Add other parameters to call, such as parity, data bits, stop bits, baud rates 

	void closePort(HANDLE handle_serial);

	std::string readLineCrLf(HANDLE handle_serial, gnss::ShutdownSequence &sq);


	class CommError: virtual public std::runtime_error{
	private:
		const unsigned int m_code;

	public:
		CommError();
		CommError(const char *what, int error_code);
		int code(void);
	};

}
