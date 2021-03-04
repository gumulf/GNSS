#include "comm.h"
#include "gnss.h"
#include "nmea.h"

#include <iostream>
#include <iomanip>



int main(){

	//if(
	//	gnss::validateChecksum("$GPRMC,010035.000,A,5742.7116,N,01201.2214,E,0.12,25.76,260221,,*36")
	//	){
	//	std::cout << "The sentence looks good!\n";
	//} else{
	//	std::cout << "Something is wrong with the sentence!\n";
	//}

	// Get connection info
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
