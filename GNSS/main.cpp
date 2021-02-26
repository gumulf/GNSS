#include "comm.h"
#include "gnss.h"
#include "nmea.h"

#include <iostream>


int main(){

	//if(
	//	gnss::validateChecksum("$GPRMC,010035.000,A,5742.7116,N,01201.2214,E,0.12,25.76,260221,,*36")
	//	){
	//	std::cout << "The sentence looks good!\n";
	//} else{
	//	std::cout << "Something is wrong with the sentence!\n";
	//}

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
