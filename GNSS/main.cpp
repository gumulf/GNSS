#include "comm.h"
#include "gnss.h"
#include "nmea.h"

#include <iostream>
#include <iomanip>

/*
	This is just for proof of concept right now

	Copyright: Ulf Andersson 2021  // TODO Find a good licence for free and open scoure code.

*/

int main(){


	// Get connection info // TODO
	LPCWSTR port{L"\\\\.\\COM3"};

	// Establish connection
	HANDLE port_handle{INVALID_HANDLE_VALUE};

	bool error_occured_reading_data{false};

	do{

		bool error_occured_opening_port{false};

		do{

			try{

				port_handle = gnss::openPort(port);

				error_occured_opening_port = false;

			}
			catch(gnss::CommError ce){

				error_occured_opening_port = true;

				gnss::closePort(port_handle);

				Sleep(1000);

			}

		} while(error_occured_opening_port);


		for(int round{0}; round < 100; ++round){

			// Read data from "GPS"
			std::string line{};

			try{
				line = gnss::readLineCrLf(port_handle);

				error_occured_reading_data = false;

			}
			catch(gnss::CommError ce){

				error_occured_reading_data = true;
				gnss::closePort(port_handle);

			}


			if(!error_occured_reading_data){
				std::cout << "Sentence: \"" << line << "\"\n";


				// Parse data	

				if(gnss::validateChecksum(line)){

					std::vector<std::string> tokens{gnss::tokenize(line, ",*")};
					gnss::nmeaParser(tokens);

				} else{

					std::cout << "Not OK!\n";

				}

				std::cout << "\n\n";

			}

		}

	} while(error_occured_reading_data);

	// Update position data

	// Present position data

	// Close connection

	gnss::closePort(port_handle);

}
