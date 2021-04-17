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

	const double PI{3.141592653589793};
	double deg_to_rad(double degrees){
		return degrees / 180.0 * PI;
	}

	double Position::distanceOnEarthSurface(const Position other){

		const double EARTH_RADIUS{6371009};

		double this_lat_rad = deg_to_rad(this->m_latitude);
		double this_long_rad = deg_to_rad(this->m_longitude);
		double other_lat_rad = deg_to_rad(other.m_latitude);
		double other_long_rad = deg_to_rad(other.m_longitude);

		double delta_lambda = abs(this_long_rad - other_long_rad);
		if(delta_lambda > PI){ delta_lambda -= PI; }

		double calculation{0};
		double temp{0};

		temp = cos(other_lat_rad) * sin(delta_lambda);
		temp *= temp;

		calculation = temp;

		temp = cos(this_long_rad) * sin(other_long_rad) - sin(this_long_rad) * cos(other_long_rad) * cos(delta_lambda);
		temp *= temp;
		calculation += temp;
		calculation = sqrt(calculation);

		temp = sin(this_long_rad) * sin(other_long_rad) + cos(this_long_rad) * cos(other_long_rad) * cos(delta_lambda);
		calculation = calculation / temp;

		calculation = atan(calculation);

		calculation *= EARTH_RADIUS;

		return calculation;
	}

}