// TODO Fix error handling, cast error?

// Note: Mostly from this document Windows Serial Port Programming, Robertson Bayer, March 30, 2008
// https://ds.opdenbrouw.nl/micprg/pdf/serial-win.pdf

#include "comm.h"
#include<iostream>


namespace gnss{

	HANDLE openPort(LPCWSTR port_name){
		HANDLE handle_serial;

		handle_serial = CreateFile(port_name,
									GENERIC_READ,
									0,
									0,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0);

		if(handle_serial == INVALID_HANDLE_VALUE){

			if(GetLastError() == ERROR_FILE_NOT_FOUND){

				CloseHandle(handle_serial);
				throw(CommError ("File not found", 3));

			} else{

				CloseHandle(handle_serial);
				throw(CommError("Error opening file", 2));
			}
		}

		DCB serial_parameters{0};

		serial_parameters.DCBlength = sizeof(serial_parameters);

		if(!GetCommState(handle_serial, &serial_parameters)){

			CloseHandle(handle_serial);
			throw(CommError("Error getting state", 4));

		}

		serial_parameters.BaudRate = CBR_4800;
		serial_parameters.ByteSize = 8;
		serial_parameters.StopBits = ONESTOPBIT;
		serial_parameters.Parity = NOPARITY;

		if(!SetCommState(handle_serial, &serial_parameters)){

			CloseHandle(handle_serial);
			throw("Error setting state", 5);

		}


		COMMTIMEOUTS timeouts{0};

		timeouts.ReadIntervalTimeout = 50;
		timeouts.ReadTotalTimeoutConstant = 50;
		timeouts.ReadTotalTimeoutMultiplier = 10;
		timeouts.WriteTotalTimeoutConstant = 50;
		timeouts.WriteTotalTimeoutMultiplier = 10;

		if(!SetCommTimeouts(handle_serial, &timeouts)){

			CloseHandle(handle_serial);
			throw(CommError("Error setting timeouts", 6));

		}

		return handle_serial;
	}

	std::string readLineCrLf(HANDLE handle_serial){

		DWORD buffert_length{1};
		char buffert{0};
		DWORD bytes_read{0};

		int max_retries_reading{10};

		std::string line{};

		bool is_line_read{false};
		bool is_cr_received{false};

		do{

			int count_retries_reading{0};

			while(!ReadFile(handle_serial, &buffert, buffert_length, &bytes_read, NULL)){

				if(count_retries_reading >= max_retries_reading){

					throw(CommError("Error reading", 7));

				}

				++count_retries_reading;

			}


			if(bytes_read == 1){

				if(!is_cr_received && buffert == '\r'){

					is_cr_received = true;

				} else if(is_cr_received && buffert == '\n'){

					is_line_read = true;

				} else if(is_cr_received){

					line.clear();
					is_cr_received = false;

					// TODO Find out if there should be an error thrown here, or something

				} else{

					line.push_back(buffert);

				}
				
			}
		} while(!is_line_read);

		return line;
	}

	int closePort(HANDLE handle_serial){

		if(handle_serial != INVALID_HANDLE_VALUE){

			CloseHandle(handle_serial);

		}

		return 1;

	}


	CommError::CommError(): std::runtime_error("An unspecified comm error occured!"), m_code{1} {}

	CommError::CommError(const char *what, int code) : std::runtime_error(what), m_code(code){}
	
	int CommError::code(){ return m_code; }


}