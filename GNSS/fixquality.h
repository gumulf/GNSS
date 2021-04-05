#pragma once

namespace gnss{
	// Quality and Fix class

	class FixQuality{

	public:
		enum class FixType{ invalid, valid, valid_2D, valid_3D };

	private:
		FixType m_fix_mode{FixType::invalid};
		unsigned int m_number_of_satellites_used{0};
		double m_pdop{-1.0};
		double m_hdop{-1.0};
		double m_vdop{-1.0};

	public:
		FixQuality();
		FixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double pdop, double hdop, double vdop);

		void setFixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double hdop);
		void setFixQuality(FixType fix_mode, unsigned int number_of_satellites_used, double pdop, double hdop, double vdop);

		void invalidateFix();

		FixType getFixMode();
		unsigned int getNumberOfSatellitesUsed();
		double getPdop();
		double getHdop();
		double getVdop();
		bool isValid();

	};
}

