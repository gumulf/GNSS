#include "gnss.h"


namespace gnss{

	/* Points in space time :) */

	Position::Position(): m_latitude{0.0}, m_longitude{0.0}{}

	Position::Position(double latitude, double longitude){

		if(latitude >= -90.0 && latitude <= 90.0){
			m_latitude = latitude;
		} else{
			throw(std::invalid_argument("Latitude must have a value between -90.0 and 90.0"));
		}

		if(longitude >= -180.0 && longitude <= 180.0){
			m_longitude = longitude;
		} else{
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
		Position::Position(latitude, longitude, meters_over_msl, meters_over_ellipsoid) {
	
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
		} else{
			throw(std::invalid_argument("Values outside allowed range, -90.0 - 90.0"));
		}
	}
	void Position::setLongitude(double longitude){

		if(longitude >= -180.0 && longitude <= 180.0){
			m_longitude = longitude;
		} else{
			throw(std::invalid_argument("Values outside allowed range, -180.0 - 180.0"));
		}
	}


	void Position::setMetersOverMsl(double meters_over_msl){ m_meters_over_msl = meters_over_msl; }
	void Position::setMetersOverEllipsoid(double meters_over_ellipsoid){ m_meters_over_ellipsoid = meters_over_ellipsoid; }
	void Position::setUtcTime(tm utc_time){ m_utc_time = utc_time; }


	/* Satelites */

	std::set<int> Satelite::m_ids{};

	Satelite::Satelite(unsigned int id){

		if(m_ids.count(id) == 0){
			m_id = id;
			m_ids.insert(id);
		} else{
			throw std::invalid_argument("ID does already exist");
		}
	}

	Satelite::Satelite(unsigned int id, unsigned int elevation, unsigned int azimuth, unsigned int snr){

		if(m_ids.count(id) == 0){
			m_id = id;
			m_ids.insert(id);
		} else{
			throw std::invalid_argument("ID does already exist");
		}

		if(elevation <= 90){
			m_elevation = elevation;
		} else{
			throw std::invalid_argument("Elevation must be less than or equal to 90");
		}

		if(azimuth < 360){
			m_azimuth = azimuth;
		} else{
			throw std::invalid_argument("Azumuth must be less than 360");
		}

		if(snr < 100){
			m_snr = snr;
		} else{
			throw std::invalid_argument("SNR must be lower than 100");
		}
	}

	unsigned int Satelite::getId(){ return m_id; }
	unsigned int Satelite::getElevation(){ return m_elevation; }
	unsigned int Satelite::getAzimuth(){ return m_azimuth; }
	unsigned int Satelite::getSnr(){ return m_snr; }

	void Satelite::setElevation(unsigned int elevation){

		if(elevation <= 90){
			m_elevation = elevation;
		} else{
			throw std::invalid_argument("Elevation must be less than or equal to 90");
		}
	}

	void Satelite::setAzimuth(unsigned int azimuth){

		if(azimuth < 360){
			m_azimuth = azimuth;
		} else{
			throw std::invalid_argument("Azumuth must be less than 360");
		}
	}

	void Satelite::setSnr(unsigned int snr){

		if(snr < 100){
			m_snr = snr;
		} else{
			throw std::invalid_argument("SNR must be lower than 100");
		}
	}

	FixQuality::FixQuality():
		m_fix_mode{FixType::invalid}, m_pdop{-1.0}, m_hdop{-1.0}, m_vdop{-1.0} {}

	FixQuality::FixQuality(FixType fix_mode, double pdop, double hdop, double vdop):
		m_fix_mode{fix_mode}, m_pdop{pdop}, m_hdop{hdop}, m_vdop{vdop} {}

	void FixQuality::setQuality(FixType fix_mode, double pdop, double hdop, double vdop) { 

		m_fix_mode = fix_mode;
		m_pdop = pdop;
		m_hdop = hdop;
		m_vdop = vdop;

	}

	FixQuality::FixType FixQuality::getFixMode(){ return m_fix_mode; }
	double FixQuality::getPdop(){ return m_pdop; }
	double FixQuality::getHdop(){ return m_hdop; }
	double FixQuality::getVdop(){ return m_vdop; }

}