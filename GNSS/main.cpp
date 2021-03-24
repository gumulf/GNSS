#include "ctrlhandler.h"
#include "comm.h"
#include "position.h"
#include "satellite.h"
#include "fixquality.h"
#include "nmea.h"

#include <iostream>
#include <iomanip>


/*
	This is just for proof of concept right now

	GNSS = Global Navigation Satellite Systems (GPS, Gallileo, Glosnass et.c.)


	Copyright: Ulf Andersson 2021  // TODO Find a good licence for free and open scoure code.
	Files: main.cpp, gnss.h, gnss.cpp, nmea.h, nmea.cpp, cpmm.h, comm.cpp

*/

int main(){
	
	// Initiation
	HANDLE port_handle{INVALID_HANDLE_VALUE};

	if(!gnss::activateCtrlHandler(&port_handle)){
		std::cerr << "Could not set CtrlHandler" << std::endl;
	}

	// Get connection info 
	// TODO Make interactive / read from call / read from config file. Including bitrate, parity bit et.c.
	LPCWSTR port{L"\\\\.\\COM3"};

	// Establish connection

	gnss::Nmea nmea{};

	bool reading_data_ok{false};
	bool reconnect{false};
	int lines_read{0}, lines_to_read{0}; // lines_to_read = 0 => no limit, Ctrl+C to stop
	int retries_opening_port{0}, max_retries_opening_port{100};
	int retries_reading_line{0}, max_retries_reading_line{10};
	int retries_reconnecting{0}, max_retries_reconnecting{100};

	do{

		reconnect = false;

		if(retries_reconnecting++ > max_retries_reconnecting){
			std::cerr << "Maxium retries reconnecting reached, aborting!" << std::endl;
			return -1;
		}

		bool port_opened_ok{false};

		do{
			if(retries_opening_port++ > max_retries_opening_port){
				std::cerr << "Maximum retries reached while trying to open port, aborting!" << std::endl;
				return -1;
			}
			try{
				port_handle = gnss::openPort(port);
				port_opened_ok = true;
			}
			catch(gnss::CommError){
				port_opened_ok = false;
				gnss::closePort(port_handle);
				Sleep(100);
			}
		} while(!port_opened_ok);


		// Read data from "GPS"	
		while((lines_read < lines_to_read || lines_to_read == 0) && !reconnect){

			retries_reading_line = 0;

			while(!reading_data_ok && !reconnect){
				std::string line{};

				try{
					line = gnss::readLineCrLf(port_handle);
					reading_data_ok = true;
				}
				catch(gnss::CommError){
					reading_data_ok = false;
				}

				if(reading_data_ok){

					//if(lines_to_read != 0){ 
						++lines_read; 
					//}
					retries_reading_line = 0;
					retries_reconnecting = 0;


					// Parse data	
					if(nmea.nmeaParser(line)){
						std::cout << "Parsed OK:   ";
						std::cout << line << "\n";
					}
					else{
						std::cout << "Not parsed:  ";
						std::cout << line << "\n";
					}
					reading_data_ok = false;

				}
				else{
					if(++retries_reading_line >= max_retries_reading_line){
						gnss::closePort(port_handle);
						reconnect = true;
						Sleep(1000);
					}
				}
			}
		}

	} while(reconnect && (lines_read < lines_to_read || lines_to_read == 0));


	// Update(/get?) position data

	// Present position data

	// Close connection

	gnss::closePort(port_handle);

	return 0;

}
