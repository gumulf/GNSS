#include "comm.h"
#include "nmea.h"

int main(){

	// Get connection info

	// Establish connection
	HANDLE hGps;
	hGps = gnss::initGps(L"\\\\.\\COM3");


	// Read data from "GPS"
	gnss::readGps(hGps, 1000);

	// Parse data

	// Update position data

	// Present position data

	// Close connection

	gnss::closeGps(hGps);

}
