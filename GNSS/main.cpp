#include "comm.h"
#include "gnss.h"
#include "nmea.h"

int main(){

	// Get connection info

	// Establish connection
	HANDLE hGnss;
	hGnss = gnss::openPort(L"\\\\.\\COM3"); 


	// Read data from "GPS"
	gnss::readPort(hGnss, 1000);

	// Parse data

	// Update position data

	// Present position data

	// Close connection

	gnss::closePort(hGnss);

}
