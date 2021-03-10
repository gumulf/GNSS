#pragma once
#include<ctime>

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
}