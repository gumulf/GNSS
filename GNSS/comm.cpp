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
				throw(CommError("File not found", 3));

			}
			else{

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
			throw(CommError("Error setting state", 5));

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

		std::string line{};

		bool is_line_read{false};

		int count_retries_reading_chars{0};
		int count_retries_reading_lines{0};

		int max_retries_reading_chars{10};
		int max_retries_reading_lines{100};


		while(true){

			count_retries_reading_chars = 0;

			while(!ReadFile(handle_serial, &buffert, buffert_length, &bytes_read, NULL)){
				if(++count_retries_reading_chars > max_retries_reading_chars){
					throw(CommError("Max retries reading chars from port exceeded!", 7));
				}
				Sleep(100);
			}

			if(bytes_read != 0){

				if(!line.empty() && line.back() == '\r' && buffert == '\n'){
					// RETURN line from function
					return line;
				}
				else if((!line.empty() && line.back() == '\r') || buffert == '\n'){
					// Either CR was the char received last round and LF is not received this round,
					// or LF is received without an CR as the char received before it

					line.clear();

					if(++count_retries_reading_lines > max_retries_reading_lines){
						throw(CommError("Max retries reading lines exceeded, lines ending in only CR or LF,  not CRLF!", 8));
					}

				}
				else{
					// Add another character to the end of the line

					line.push_back(buffert);
				}

			}
		}

	}


	int closePort(HANDLE handle_serial){

		if(handle_serial != INVALID_HANDLE_VALUE && handle_serial != NULL){
			CloseHandle(handle_serial);
		}

		return 1;
	}


	CommError::CommError(): std::runtime_error("An unspecified comm error occured!"), m_code{1} {}

	CommError::CommError(const char *what, int code) : std::runtime_error(what), m_code(code){}

	int CommError::code(){ return m_code; }


}