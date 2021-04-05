#include "nmea.h"
#include <cmath>
#include <string>
#include <stdexcept>


namespace gnss{

	// Declarations of local functions

	bool isChecksumValid(std::string sentence);
	std::vector<std::string>  tokenize(std::string str, std::string start_chars, std::string separators);
	bool parseGga(std::vector<std::string> sentence_tokens, gnss::Position &position, gnss::FixQuality &fix_quality);
	tm extractTime(std::string time_string);
	double extractCoordinate(std::string coordinate_string, double lower_limit, double upper_limit);

	// Parser
	bool Nmea::nmeaParser(std::string sentence){

		if(!isChecksumValid(sentence)){
			return false;
		}

		std::vector<std::string> sentence_tokens{tokenize(sentence, "$!", ",*")};
		std::string sentenceType{sentence_tokens.front().substr(3, 3)};

		if(sentenceType == "GGA"){
			gnss::Position pos{};
			gnss::FixQuality fq{};

			bool parse_result{parseGga(sentence_tokens, std::ref(pos), std::ref(fq))};

			if(parse_result){

				{
					std::lock_guard<std::mutex> pos_lock(m_pos_mx);
					m_position = pos;
				}
				{
					std::lock_guard<std::mutex> fix_lock(m_fix_mx);
					m_fix_quality = fq;
				}
			}

			return parse_result;
		}
		else{
			return false;
		}

	}

	gnss::Position Nmea::getPosition(){ 
		std::lock_guard<std::mutex> pos_lock(m_pos_mx);
		return m_position;
	}

	gnss::FixQuality Nmea::getFixQuality(){
		std::lock_guard<std::mutex> fix_lock(m_fix_mx);
		return m_fix_quality;
	}

	std::vector<gnss::Satellite> Nmea::getSatellites(){
		std::lock_guard<std::mutex> sat_lock(m_sat_mx);
		return m_satellites;
	}


	/**********************************************************************************/

	bool parseGga(std::vector<std::string> sentence_tokens, gnss::Position &position, gnss::FixQuality &fix_quality){

		try{
			if(std::stoi(sentence_tokens.at(6)) >= 1){

				Position current_position{};
				tm current_utc_time{};

				try{
					current_utc_time = extractTime(sentence_tokens.at(1));
				}
				catch(std::out_of_range){
					return false;
				}
				catch(...){ throw; }

				current_position.setUtcTime(current_utc_time);

				try{
					double latitude{extractCoordinate(sentence_tokens.at(2), -90.0, 90.0)};
					double longitude{extractCoordinate(sentence_tokens.at(4), -180.0, 180.0)};
					current_position.setPosition(latitude, longitude);
				}
				catch(std::out_of_range){
					return false;
				}
				catch(std::invalid_argument){
					return false;
				}
				catch(...){ throw; }

				double altitudeMsl{std::stod(sentence_tokens.at(9))};
				double altitudeEllipsoid{std::stod(sentence_tokens.at(9)) + std::stod(sentence_tokens.at(11))};

				// This should not be anything other than in meters, but just in case
				if((sentence_tokens.at(10) == "M" || sentence_tokens.at(10) == "m") && (sentence_tokens.at(12) == "M" || sentence_tokens.at(12) == "m")){
					current_position.setHeight(altitudeMsl, altitudeEllipsoid);
				}
				else{
					return false;
				}

				// TODO Needs to be atomic -->
				position = current_position; 

				fix_quality.setFixQuality(
					gnss::FixQuality::FixType::valid,
					static_cast<unsigned int>(std::stoi(sentence_tokens.at(7))), // Satellites used for fix
					std::stod(sentence_tokens.at(8))); // hdop

				// <--

				return true;
			}
			else{
				// Fixation not available!
				// TODO, set position to invalid???
				return false; 
			}

		}
		catch(const std::invalid_argument){
			return false;
		}
		catch(const std::out_of_range){
			return false;
		}
		catch(...){
			throw;
		}
	}


	// Check the sentence for corruption by checking the checksum
	bool isChecksumValid(std::string sentence){

		// Sentence should start with '$' or '!'.
		size_t firstChar{sentence.find_first_of("$!")};

		//	If there is no '$' or '!', the test fails
		if(firstChar == std::string::npos){
			return false;
		}

		// The data is after first character, checked above, and before '*'.
		// The checksum is after the '*'.
		size_t dataEnd{sentence.find('*')};
		if(dataEnd == std::string::npos || firstChar > dataEnd){
			return false;
		}

		// Calculation of checksum
		unsigned char checksum{0};
		for(size_t pos{firstChar + 1}; pos < dataEnd; ++pos){
			checksum ^= sentence.at(pos);
		}

		// Compare calculated checksum against checksum at end of sentence (that is in hexadecimal notation)
		if(std::stoi(sentence.substr(dataEnd + 1, std::string::npos), nullptr, 16) == static_cast<int>(checksum)){
			return true;
		}
		else{
			return false;
		}

	}

	// Tokenize the sentence
	std::vector<std::string>  tokenize(std::string str, std::string start_chars, std::string separators){

		std::vector<std::string> tokens;
		size_t start{str.find_first_of(start_chars, 0)}, end{str.find_first_of(separators, start)};

		while(end != std::string::npos){
			tokens.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find_first_of(separators, start);
		}

		if(start != str.length()){
			tokens.push_back(str.substr(start));
		}

		return tokens;
	}

	// Get time from string, with checking
	tm extractTime(std::string time_string){

		int hour{std::stoi(time_string.substr(0, 2))};
		int minute{std::stoi(time_string.substr(2, 2))};
		int second{std::stoi(time_string.substr(4, 2))};



		if(hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 60){

			tm time_object{};
			time_object.tm_hour = hour;
			time_object.tm_min = minute;
			time_object.tm_sec = second;

			return time_object;

		}
		else{
			throw std::out_of_range("");
		}

	}

	// Get coordinate, with checking
	double extractCoordinate(std::string coordinate_string, double lower_limit, double upper_limit){

		size_t decimalPosition{coordinate_string.find('.')};

		if(decimalPosition == std::string::npos){
			throw std::invalid_argument("No decimal point!");
		}

		double coordinate{0};
		coordinate += std::stod(coordinate_string.substr(0, decimalPosition - 2));
		coordinate += (std::stod(coordinate_string.substr(decimalPosition - 2, std::string::npos)) / 60.0);

		if(coordinate < lower_limit && coordinate > upper_limit){
			throw std::out_of_range("Coordinates are out of range!");
		}

		return coordinate;
	}
}