#pragma once
#include<ctime>

namespace gnss{

	class Position{

		double m_latitude{0.0};   // North-South	(90.0 - -90.0 degrees)
		double m_longitude{0.0};  // East-West	(180.0 - -180.0 degrees)
		double m_meters_over_msl{0.0};  // Hight over mean sealevel
		double m_meters_over_ellipsoid{0.0}; // Hight over Ellipsoid
		tm m_utc_time{NULL};
		bool m_is_position_valid{false};
		bool m_is_hight_valid{false};
		bool m_is_time_valid{false};

	public:
		Position();
		Position(double latitude, double longitude);
		Position(double latitude, double longitude, tm utc_time);
		Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid);
		Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid, tm utc_time);

		void setPosition(double latitude, double longitude);
		void setHeight(double meters_over_msl, double meters_over_ellipsoid);
		void setUtcTime(tm utc_time);

		bool isPositionValid();
		bool isHightValid();
		bool isTimeValid();
		bool isAllValid();

		double getLatitude();
		double getLongitude();
		double getMetersOverMsl();
		double getMetersOverEllipsoid();
		tm getUtcTime();

		double distanceOnEarthSurface(const Position other);


	};
}