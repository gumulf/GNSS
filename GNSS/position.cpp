#include "position.h"
#include<stdexcept>

namespace gnss{

	/* Points in space time :) */

	Position::Position(): m_latitude{0.0}, m_longitude{0.0}{}

	Position::Position(double latitude, double longitude){

		if(latitude >= -90.0 && latitude <= 90.0){
			m_latitude = latitude;
		}
		else{
			throw(std::invalid_argument("Latitude must have a value between -90.0 and 90.0"));
		}

		if(longitude >= -180.0 && longitude <= 180.0){
			m_longitude = longitude;
		}
		else{
			throw(std::invalid_argument("Longitude must have a value between - 180.0 - 180.0"));
		}
	}

	Position::Position(double latitude, double longitude, tm utc_time):
		Position::Position(latitude, longitude){

		m_utc_time = utc_time;

	}

	Position::Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid):
		Position::Position(latitude, longitude){

		m_meters_over_msl = meters_over_msl;
		m_meters_over_ellipsoid = meters_over_ellipsoid;

	}

	Position::Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid, tm utc_time):
		Position::Position(latitude, longitude, meters_over_msl, meters_over_ellipsoid){

		m_utc_time = utc_time;

	}

	double Position::getLatitude(){ return m_latitude; }
	double Position::getLongitude(){ return m_longitude; }
	double Position::getMetersOverMsl(){ return m_meters_over_msl; }
	double Position::getMetersOverEllipsoid(){ return m_meters_over_ellipsoid; }
	tm Position::getUtcTime(){ return m_utc_time; }

	void Position::setLatitude(double latitude){

		if(latitude >= -90.0 && latitude <= 90.0){
			m_latitude = latitude;
		}
		else{
			throw(std::invalid_argument("Values outside allowed range, -90.0 - 90.0"));
		}
	}
	void Position::setLongitude(double longitude){

		if(longitude >= -180.0 && longitude <= 180.0){
			m_longitude = longitude;
		}
		else{
			throw(std::invalid_argument("Values outside allowed range, -180.0 - 180.0"));
		}
	}


	void Position::setMetersOverMsl(double meters_over_msl){ m_meters_over_msl = meters_over_msl; }
	void Position::setMetersOverEllipsoid(double meters_over_ellipsoid){ m_meters_over_ellipsoid = meters_over_ellipsoid; }
	void Position::setUtcTime(tm utc_time){ m_utc_time = utc_time; }
}