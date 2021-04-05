#include "position.h"
#include<stdexcept>

namespace gnss{

	/* Points in space time :) */

	Position::Position(): m_latitude{0.0}, m_longitude{0.0}, m_is_position_valid(false){}

	Position::Position(double latitude, double longitude){
		try{
			setPosition(latitude, longitude);
		}
		catch(...){
			throw;
		}
	}

	Position::Position(double latitude, double longitude, tm utc_time):
		Position::Position(latitude, longitude){
		setUtcTime(utc_time);
	}

	Position::Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid):
		Position::Position(latitude, longitude){
		setHeight(meters_over_msl, meters_over_ellipsoid);
	}

	Position::Position(double latitude, double longitude, double meters_over_msl, double meters_over_ellipsoid, tm utc_time):
		Position::Position(latitude, longitude, meters_over_msl, meters_over_ellipsoid){
		setUtcTime(utc_time);
	}

	void Position::setPosition(double latitude, double longitude){

		if((latitude < -90.0 || latitude > 90.0) && (longitude < -180.0 && longitude > 180.0)){
			throw(std::invalid_argument("Values outside allowed range for latitude (-90.0 - 90.0) and for longitude (-180.0 - 180.0)!"));
		}
		if(latitude < -90.0 || latitude > 90.0){
			throw(std::invalid_argument("Values outside allowed range for latitude, -90.0 - 90.0!"));
		}
		if(longitude < -180.0 && longitude > 180.0){
			throw(std::invalid_argument("Values outside allowed range for longitude, -180.0 - 180.0!"));
		}

		m_latitude = latitude;
		m_longitude = longitude;
		m_is_position_valid = true;
	}

	void Position::setHeight(double meters_over_msl, double meters_over_ellipsoid){
		m_meters_over_msl = meters_over_msl;
		m_meters_over_ellipsoid = meters_over_ellipsoid;
		m_is_hight_valid = true;
	}

	void Position::setUtcTime(tm utc_time){
		m_utc_time = utc_time;
		m_is_time_valid = true;
	}

	bool Position::isPositionValid(){ return m_is_position_valid; }
	bool Position::isHightValid(){ return m_is_hight_valid; }
	bool Position::isTimeValid(){ return m_is_time_valid; }
	bool Position::isAllValid(){ return m_is_position_valid && m_is_hight_valid && m_is_time_valid; }

	double Position::getLatitude(){ return m_latitude; }
	double Position::getLongitude(){ return m_longitude; }
	double Position::getMetersOverMsl(){ return m_meters_over_msl; }
	double Position::getMetersOverEllipsoid(){ return m_meters_over_ellipsoid; }
	tm Position::getUtcTime(){ return m_utc_time; }

}