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


	// Get connection info 
	// TODO Make interactive / read from call / read from config file. Including bitrate, parity bit et.c.
	LPCWSTR port{L"\\\\.\\COM3"};

	// Establish connection
	HANDLE port_handle{INVALID_HANDLE_VALUE};

	gnss::Nmea nmea{};

	bool error_occured_reading_data{false};
	int round{0}, no_of_rounds{100};

	do{
		bool error_occured_opening_port{false};

		do{
			try{
				port_handle = gnss::openPort(port);
				error_occured_opening_port = false;
			}
			catch(gnss::CommError){
				error_occured_opening_port = true;
				gnss::closePort(port_handle);
				Sleep(1000);
			}
		} while(error_occured_opening_port);


		while(round < no_of_rounds){
			// Read data from "GPS"

			std::string line{};

			try{
				line = gnss::readLineCrLf(port_handle);
				error_occured_reading_data = false;
			}
			catch(gnss::CommError){
				error_occured_reading_data = true;
				gnss::closePort(port_handle);
			}

			if(!error_occured_reading_data){

				std::cout << "Sentence: \"" << line << "\"\n";
	
				// Parse data	
				if(nmea.nmeaParser(line)){
					std::cout << "OK!\n";
				} else{
					std::cout << "Not OK!\n";
				}
				std::cout << "\n\n";
			}

			++round;
		}
	} while(error_occured_reading_data);

	// Update position data

	// Present position data

	// Close connection

	gnss::closePort(port_handle);

	return 0;

}
