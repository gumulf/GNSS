#include "satellite.h"
#include <stdexcept>

namespace gnss{

	/* Satellites */

	std::set<unsigned int> Satellite::m_ids{};

	Satellite::Satellite(unsigned int id){

		if(m_ids.count(id) == 0){
			m_id = id;
			m_ids.insert(id);
		}
		else{
			throw std::invalid_argument("ID does already exist");
		}
	}

	Satellite::Satellite(unsigned int id, unsigned int elevation, unsigned int azimuth, unsigned int snr){

		if(m_ids.count(id) == 0){
			m_id = id;
			m_ids.insert(id);
		}
		else{
			throw std::invalid_argument("ID does already exist");
		}

		if(elevation <= 90){
			m_elevation = elevation;
		}
		else{
			throw std::invalid_argument("Elevation must be less than or equal to 90");
		}

		if(azimuth < 360){
			m_azimuth = azimuth;
		}
		else{
			throw std::invalid_argument("Azumuth must be less than 360");
		}

		if(snr < 100){
			m_snr = snr;
		}
		else{
			throw std::invalid_argument("SNR must be lower than 100");
		}
	}

	unsigned int Satellite::getId(){ return m_id; }
	unsigned int Satellite::getElevation(){ return m_elevation; }
	unsigned int Satellite::getAzimuth(){ return m_azimuth; }
	unsigned int Satellite::getSnr(){ return m_snr; }

	void Satellite::setElevation(unsigned int elevation){

		if(elevation <= 90){
			m_elevation = elevation;
		}
		else{
			throw std::invalid_argument("Elevation must be less than or equal to 90");
		}
	}

	void Satellite::setAzimuth(unsigned int azimuth){

		if(azimuth < 360){
			m_azimuth = azimuth;
		}
		else{
			throw std::invalid_argument("Azumuth must be less than 360");
		}
	}

	void Satellite::setSnr(unsigned int snr){

		if(snr < 100){
			m_snr = snr;
		}
		else{
			throw std::invalid_argument("SNR must be lower than 100");
		}
	}

	bool Satellite::doesSatelliteIdExist(unsigned int id){ return m_ids.count(id) == 1; }
}