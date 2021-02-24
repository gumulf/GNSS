#pragma once

// TODO Implement class and functions
// Should include datatypes for position, time, satelite data et.c.

namespace gnss{

	class Position{

		double m_latitude{0.0};   // North-South	(90.0 - -90.0 degrees)
		double m_longitude{0.0};  // East-West	(180.0 - -180.0 degrees)

	public:

		Position();

		Position(double latitude, double longitude);

		double getLatitude();
		double getLongitude();

		void setLatitude(double latitude);
		void setLongitude(double longitude);

	};


	// Time

	// Satelite data type

	// Fix type

	// Quality (Struct with the different quality types)

}