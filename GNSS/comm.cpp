// TODO Fix error handling, cast error?

// Note: Mostly from this document Windows Serial Port Programming, Robertson Bayer, March 30, 2008
// https://ds.opdenbrouw.nl/micprg/pdf/serial-win.pdf

#include "comm.h"

#include<iostream> // TODO Remove this 


namespace gnss{

	HANDLE openPort(LPCWSTR port_name){
		HANDLE port_handle{INVALID_HANDLE_VALUE};

		port_handle = CreateFile(port_name,
											GENERIC_READ|GENERIC_WRITE,
											0,
											0,
											OPEN_EXISTING,
											FILE_FLAG_OVERLAPPED,               //FILE_ATTRIBUTE_NORMAL,
											0);

		if(port_handle == INVALID_HANDLE_VALUE){

			if(GetLastError() == ERROR_FILE_NOT_FOUND){

				CloseHandle(port_handle);
				throw(CommError("File not found", 3));

			}
			else{

				CloseHandle(port_handle);
				throw(CommError("Error opening file", 2));
			}
		}

		DCB serial_parameters{0};

		serial_parameters.DCBlength = sizeof(serial_parameters);

		if(!GetCommState(port_handle, &serial_parameters)){

			CloseHandle(port_handle);

			throw(CommError("Error getting state", 4));

		}

		serial_parameters.BaudRate = CBR_4800;
		serial_parameters.ByteSize = 8;
		serial_parameters.StopBits = ONESTOPBIT;
		serial_parameters.Parity = NOPARITY;

		if(!SetCommState(port_handle, &serial_parameters)){

			CloseHandle(port_handle);
			throw(CommError("Error setting state", 5));

		}


		// If there are bytes in the input buffer, return immediately with them
		// If there are no bytes in the input buffer, wait until a byte arrives and the return with it
		// It no bytes arrive within ReadTotalTimeoutConstant (=1000) milliseconds, ReadFile times out
		// 		timeouts.ReadIntervalTimeout = MAXDWORD;
		// 		timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
		// 		timeouts.ReadTotalTimeoutConstant = 1000;

		COMMTIMEOUTS timeouts{};

		timeouts.ReadIntervalTimeout = MAXDWORD;
		timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
		timeouts.ReadTotalTimeoutConstant = 1000;
		timeouts.WriteTotalTimeoutMultiplier = 10;
		timeouts.WriteTotalTimeoutConstant = 50;

		if(!SetCommTimeouts(port_handle, &timeouts)){

			CloseHandle(port_handle);
			throw(CommError("Error setting timeouts", 6));

		}

		return port_handle;
	}

	std::string readLineCrLf(HANDLE handle_serial, ShutdownSequence &sq){

		DWORD buffert_length{1};
		char buffert{0};
		DWORD bytes_read{0};

		std::string line{};
		bool line_read{false};

		int retries_reading_chars_count{0};
		int retries_reading_lines_count{0};
		int read_timeouts_count{0};

		int max_retries_reading_chars{10};
		int max_retries_reading_lines{10};
		int max_read_timeouts{50};


		while(!line_read && !sq.isActivated()){

			retries_reading_chars_count = 0;

			OVERLAPPED overlapped_read{0};
			overlapped_read.Offset = 0;
			overlapped_read.OffsetHigh = 0;
			overlapped_read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			bool read_char{false};

			SetCommMask(handle_serial, EV_RXCHAR);

			// TODO Cleanup this mess ...

			while(!read_char && !sq.isActivated()){

				if(!ReadFile(handle_serial, &buffert, 1, NULL, &overlapped_read)){

					DWORD last_error_read_file = GetLastError();

					if(last_error_read_file == ERROR_IO_PENDING){
						// Async read pending

						while(!read_char && !sq.isActivated()){

							if(GetOverlappedResultEx(handle_serial, &overlapped_read, &bytes_read, 5000, FALSE)){
								// Returned, check if read byte
								if(overlapped_read.InternalHigh == 1){
									read_char = true;
								}

							}
							else{
								DWORD last_error_get_result = GetLastError();

								if(last_error_get_result == WAIT_TIMEOUT){
									// Timeout occured 
									if(read_timeouts_count++ > max_read_timeouts){
										CancelIo(handle_serial);
										throw(CommError("Max read timeouts count reached!", 9));
									}
								}
								else if(last_error_get_result == ERROR_IO_INCOMPLETE){
									// dwMilliseconds is zero and the operation is still in progress, 


								}
								else if(last_error_get_result == WAIT_IO_COMPLETION){
									// dwMilliseconds is nonzero, and an I/O completion routine or APC is queued

								}
								else{
									// Why are we here???
									if(++retries_reading_chars_count > max_retries_reading_chars){
										CancelIo(handle_serial);
										throw(CommError("Max retries reading chars from port exceeded!", 7));
									}
									Sleep(100);
								}
							}
						}
					}
					else{
						if(++retries_reading_chars_count > max_retries_reading_chars){
							CancelIo(handle_serial);
							throw(CommError("Max retries reading chars from port exceeded!", 7));
						}
						Sleep(100);

					}
				}
				else{

					// Read syncro
					if(overlapped_read.InternalHigh == 1){
						bytes_read = true;
					}
					else{
						if(++retries_reading_chars_count > max_retries_reading_chars){
							CancelIo(handle_serial);
							throw(CommError("Max retries reading chars from port exceeded!", 7));
						}
					}
				}
			}

			// Check if program is shutting down
			if(sq.isActivated()){
				CancelIo(handle_serial);
				return "";
			}


			// Time to read byte
			read_timeouts_count = 0;
			retries_reading_chars_count = 0;

			if(!line.empty() && line.back() == '\r' && buffert == '\n'){
				retries_reading_lines_count = 0; // Just for clearity
				line_read = true;
			}
			else if((!line.empty() && line.back() == '\r') || buffert == '\n'){
				// Either CR was the char received last round and LF is not received this round,
				// or LF is received without an CR as the char received before it

				line.clear();

				if(++retries_reading_lines_count > max_retries_reading_lines){
					throw(CommError("Max retries reading lines exceeded, lines ending in only CR or LF,  not CRLF!", 8));
				}
			}
			else{
				// Add another character to the end of the line
				line.push_back(buffert);
			}

		}

		return line;
	}


	void closePort(HANDLE handle_serial){

		if(handle_serial != INVALID_HANDLE_VALUE){
			try{
				CloseHandle(handle_serial);
			}
			catch(...){
				// TODO Find a better solution than catch all exceptions
				// TODO Remove write to std::cerr
				std::cerr << "Error closing handle!" << std::endl;
			}
		}

		return;
	}


	CommError::CommError(): std::runtime_error("An unspecified comm error occured!"), m_code{1} {}

	CommError::CommError(const char *what, int code) : std::runtime_error(what), m_code(code){}

	int CommError::code(){ return m_code; }


}