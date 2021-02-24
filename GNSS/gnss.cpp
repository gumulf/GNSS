#include "gnss.h"

namespace gnss{

	Position::Position(): m_latitude{0.0}, m_longitude{0.0}{}

	Position::Position(double latitude, double longitude){

		if(latitude >= -90.0 && latitude <= 90.0 && longitude >= -180.0 && longitude <= 180.0){
			m_latitude = latitude;
			m_longitude = longitude;
		} else{
			// TODO Create error
		}

	}

	double Position::getLatitude(){
		return m_latitude;
	}
	double Position::getLongitude(){
		return m_longitude;
	}

	void Position::setLatitude(double latitude){
		if(latitude >= -90.0 && latitude <= 90.0){
			m_latitude = latitude;
		} else{
			// TODO Create Error
		}
	
	}
	void Position::setLongitude(double longitude){
		if(longitude >= -180.0 && longitude <= 180.0){
			m_longitude = longitude;
		} else{
			// TODO Create Error
		}
	
	}



}