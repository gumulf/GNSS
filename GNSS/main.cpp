#include "ctrlhandler.h"
#include "comm.h"
#include "position.h"
#include "satellite.h"
#include "fixquality.h"
#include "nmea.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <vector>


/*
	This is just for proof of concept right now

	GNSS = Global Navigation Satellite Systems (GPS, Gallileo, Glosnass et.c.)


	Copyright: Ulf Andersson 2021  // TODO Find a good licence for free and open scoure code.
	Files: main.cpp, gnss.h, gnss.cpp, nmea.h, nmea.cpp, cpmm.h, comm.cpp

*/

int connectAndRead(LPCWSTR port, gnss::Nmea &nmea, gnss::ShutdownSequence &sq);
void presentData(gnss::Nmea &nmea, gnss::ShutdownSequence &sq);
std::mutex cout_mx;


int main(){
	
	// Initiation
	gnss::ShutdownSequence sq;
	gnss::Nmea nmea;

	if(!gnss::activateCtrlHandler(std::ref(sq))){
		std::cerr << "Could not set CtrlHandler" << std::endl;
		return 0;
	}

	// Get connection info 
	// TODO Make interactive / read from call / read from config file. Including bitrate, parity bit et.c.
	LPCWSTR port{L"\\\\.\\COM3"};


	std::thread reading(connectAndRead, port, std::ref(nmea), std::ref(sq));
	std::thread presenting(presentData, std::ref(nmea), std::ref(sq));



	// Update(/get?) position data

	// Present position data

	// Shutdown program

	sq.waitForActivation();
	{
		std::lock_guard<std::mutex> cout_lock(cout_mx);
		std::cout << "\n\n === Program shutting down! === \n\n\n";
	}
	presenting.join();
	reading.join();

	return 0;

}


int connectAndRead(LPCWSTR port, gnss::Nmea &nmea, gnss::ShutdownSequence &sq){

	int lines_read{0}, lines_to_read{0}; // lines_to_read = 0 => no limit, Ctrl+C to stop
	int retries_opening_port{0}, max_retries_opening_port{100};
	int retries_reading_line{0}, max_retries_reading_line{10};
	int retries_reconnecting{0}, max_retries_reconnecting{100};

	HANDLE port_handle{INVALID_HANDLE_VALUE};
	//gnss::Nmea nmea{};
	bool reading_data_ok{false};
	bool reconnect{true};

	while(reconnect && (lines_read < lines_to_read || lines_to_read == 0) && !sq.isActivated()){

		/***************************************************************************************************/

	// Establish connection
		reconnect = false;

		if(retries_reconnecting++ > max_retries_reconnecting){
			std::cerr << "Maxium retries reconnecting reached, aborting!" << std::endl;
			return -1;
		}

		/***************************************************************************************************/

		bool port_opened_ok{false};

		while(!port_opened_ok && !sq.isActivated()){
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
				Sleep(100);
			}
		};

		/***************************************************************************************************/

		// Read data from "GPS"	
		while((lines_read < lines_to_read || lines_to_read == 0) && !reconnect && !sq.isActivated()){ // lines_to_read = 0 => no limit, Ctrl+C to stop

			retries_reading_line = 0;

			while(!reading_data_ok && !reconnect && !sq.isActivated()){
				std::string line{};

				try{
					line = gnss::readLineCrLf(port_handle, std::ref(sq));
					reading_data_ok = true;
				}
				catch(gnss::CommError){
					reading_data_ok = false;
				}

				if(reading_data_ok){

					++lines_read;
					retries_reading_line = 0;
					retries_reconnecting = 0;

					// Parse data	
					nmea.nmeaParser(line);

					reading_data_ok = false;

				}
				else{
					if(++retries_reading_line >= max_retries_reading_line){
						gnss::closePort(port_handle);
						reconnect = true;
						Sleep(500);
					}
				}
			}

			/***************************************************************************************************/

		}

	};

	// Close connection
	gnss::closePort(port_handle);
	return 0;
}

void presentData(gnss::Nmea &nmea, gnss::ShutdownSequence &sq){

	gnss::Position previous_position{};
	double distance{0.0};

	while(!sq.isActivated()){
		gnss::Position pos{nmea.getPosition()};
		gnss::FixQuality fq{nmea.getFixQuality()};
		std::vector<gnss::Satellite> sat{nmea.getSatellites()};
		tm utc_time = pos.getUtcTime();

		const int buf_size{10};
		char utc_time_buf[buf_size];
		strftime(utc_time_buf, buf_size, "%T", &utc_time);
			

		if(fq.getFixMode() == gnss::FixQuality::FixType::invalid){
			{
				std::lock_guard<std::mutex> cout_lock(cout_mx);
				std::cout << utc_time_buf << " - ";
				std::cout << "Position fix not available!\n";

			}
		}
		else{
			if(previous_position.isPositionValid()){
				distance = pos.distanceOnEarthSurface(previous_position);
			}
			else{
				previous_position = pos;
			}
			{
				std::lock_guard<std::mutex> cout_lock(cout_mx);
				std::cout << utc_time_buf << " - ";
				std::cout << std::fixed << std::setprecision(5);
				std::cout << "Lat: " << pos.getLatitude() << " Long: " << pos.getLongitude();
				std::cout << std::setprecision(1);
				std::cout << " - Dist to start: " << distance << " m\n";
			}
		}
		Sleep(1000);
	}

}

