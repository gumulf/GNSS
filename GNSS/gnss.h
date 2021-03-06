#pragma once
#include<set>
#include<stdexcept>
#include<ctime>

// TODO Implement class and functions
// Should include datatypes for position, time, satelite data et.c.

namespace gnss{

	class Position{

		double m_latitude{0.0};   // North-South	(90.0 - -90.0 degrees)
		double m_longitude{0.0};  // East-West	(180.0 - -180.0 degrees)
		double m_meters_over_msl{0.0};  // Hight over mean sealevel
		double m_meters_over_ellipsoid{0.0}; // Hight over Ellipsoid
		tm m_utc_time{NULL};

	public:
		Position();
		Position(double latitude, double longitude);
		Position(double latitude, double longitude, tm utc_time);
		Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid);
		Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid, tm utc_time);

		double getLatitude();
		double getLongitude();
		double getMetersOverMsl();
		double getMetersOverEllipsoid();
		tm getUtcTime();

		void setLatitude(double latitude);
		void setLongitude(double longitude);
		void setMetersOverMsl(double meters_over_msl);
		void setMetersOverEllipsoid(double meters_over_ellipsoid);
		void setUtcTime(tm utc_time);



	};


	// Time

	// Satelite data type
	class Satelite{

		unsigned int m_id{0};
		unsigned int m_elevation{0};
		unsigned int m_azimuth{0};
		unsigned int m_snr{0};
		static std::set<int> m_ids;

	public:
		Satelite(unsigned int id);
		Satelite(unsigned int id, 
					unsigned int elevation, 
					unsigned int azimuth, 
					unsigned int snr);

		unsigned int getId();
		unsigned int getElevation();
		unsigned int getAzimuth();
		unsigned int getSnr();

		void setElevation(unsigned int elevation);
		void setAzimuth(unsigned int azimuth);
		void setSnr(unsigned int snr);

	};

	// Quality and Fix type

	class FixQuality{

	public:
		enum class FixType{ invalid, valid_2D = 2, valid_3D = 3 };

	private:
		FixType m_fix_mode{FixType::invalid};
		double m_pdop{-1.0};
		double m_hdop{-1.0};
		double m_vdop{-1.0};

	public:
		FixQuality();
		FixQuality(FixType fix_mode, double pdop, double hdop, double vdop);
		void setQuality(FixType fix_mode, double pdop, double hdop, double vdop);
		FixType getFixMode();
		double getPdop();
		double getHdop();
		double getVdop();
		
	};

}