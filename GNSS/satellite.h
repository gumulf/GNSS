#pragma once
#include <set>

namespace gnss{

	// Satellite data type
	class Satellite{

		unsigned int m_id{0};
		unsigned int m_elevation{0};
		unsigned int m_azimuth{0};
		unsigned int m_snr{0};
		static std::set<unsigned int> m_ids;

	public:
		Satellite(unsigned int id);
		Satellite(unsigned int id,
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

		bool doesSatelliteIdExist(unsigned int);

	};
}

