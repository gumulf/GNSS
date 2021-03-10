#include "fixquality.h"

namespace gnss{
	// Quality of the fix

	FixQuality::FixQuality():
		m_fix_mode{FixType::invalid}, m_pdop{-1.0}, m_hdop{-1.0}, m_vdop{-1.0} {}

	FixQuality::FixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double pdop, double hdop, double vdop) :
		m_fix_mode{fix_mode}, m_number_of_satellites_used{number_of_satellites_used}, m_pdop{pdop}, m_hdop{hdop}, m_vdop{vdop} {}

	void FixQuality::setFixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double hdop){
		m_fix_mode = fix_mode;
		m_number_of_satellites_used = number_of_satellites_used;
		m_hdop = hdop;
	}

	void FixQuality::setFixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double pdop, double hdop, double vdop){
		m_fix_mode = fix_mode;
		m_number_of_satellites_used = number_of_satellites_used;
		m_pdop = pdop;
		m_hdop = hdop;
		m_vdop = vdop;
	}

	FixQuality::FixType FixQuality::getFixMode(){ return m_fix_mode; }
	unsigned int FixQuality::getNumberOfSatellitesUsed(){ return m_number_of_satellites_used; }
	double FixQuality::getPdop(){ return m_pdop; }
	double FixQuality::getHdop(){ return m_hdop; }
	double FixQuality::getVdop(){ return m_vdop; }

}