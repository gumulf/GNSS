#pragma once
#include "position.h"
#include "satellite.h"
#include "fixquality.h"
#include <string>
#include <vector>

namespace gnss{
	class Nmea{

	private:
		gnss::Position m_position{};
		gnss::FixQuality m_fix_quality{};
		std::vector<gnss::Satellite> m_satellites{};

	public:
		bool nmeaParser(std::string sentence); // TODO Should this be renamed? It updates the data aswell
		gnss::Position getPosition();
		gnss::FixQuality getFixQuality();
		std::vector<gnss::Satellite> getSatellites();
	};
}