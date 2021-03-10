#include "nmea.h"
#include <cmath>
#include <string>
#include <stdexcept>


#include <iostream> // TODO Remove after initial testing and developemnt

namespace gnss{

	// Function declarations

	bool validateChecksum(std::string sentence);
	std::vector<std::string>  tokenize(std::string str, std::string separators);
	bool parseGga(std::vector<std::string> sentence_tokens, gnss::Position &position, gnss::FixQuality &fix_quality);

	// Parser
	// TODO Define and implement how data is returned/updated

	bool Nmea::nmeaParser(std::string sentence){

		if(!validateChecksum(sentence)){ return false; }

		std::vector<std::string> sentence_tokens{tokenize(sentence, ",*")};

		std::string sentenceType{sentence_tokens.front().substr(3, 3)};

		std::cout << "The sentence type is: " << sentenceType << "\n";

		if(sentenceType == "GGA"){
			if(parseGga(sentence_tokens, m_position, m_fix_quality)){
				std::cout << "Parsing of GGA done OK\n";
			} else{
				std::cout << "Could not parse GGA\n";
			}

		} else{
			std::cout << "Parsing of type " << sentence_tokens.at(0) << " in not implemented!\n";

			// Unkown sentence type
			// TODO Do good stuff here
		}

		return true;
		
	}

	gnss::Position Nmea::getPosition(){ return m_position; }
	gnss::FixQuality Nmea::getFixQuality(){ return m_fix_quality; }
	std::vector<gnss::Satellite> Nmea::getSatellites(){ return m_satellites; }


	/**********************************************************************************/

	bool parseGga(std::vector<std::string> sentence_tokens, gnss::Position &position, gnss::FixQuality &fix_quality){

		try{
			if(std::stoi(sentence_tokens.at(6)) >= 1){

				Position current_position{};
				tm current_utc_time{};
				bool valid_utc_time{false};

				int hour{std::stoi(sentence_tokens.at(1).substr(0, 2))};
				int minute{std::stoi(sentence_tokens.at(1).substr(2, 2))};
				int second{std::stoi(sentence_tokens.at(1).substr(4, 2))};

				if(hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && second >= 0 && second <= 60){
					current_utc_time.tm_hour = hour;
					current_utc_time.tm_min = minute;
					current_utc_time.tm_sec = second;
					current_position.setUtcTime(current_utc_time);
					valid_utc_time = true;
				}

				bool valid_position{true};

				double latitude{std::stod(sentence_tokens.at(2).substr(0, 2)) + (std::stod(sentence_tokens.at(2).substr(2, std::string::npos)) / 60.0)};
				double longitude{std::stod(sentence_tokens.at(4).substr(0, 3)) + (std::stod(sentence_tokens.at(4).substr(3, std::string::npos)) / 60.0)};

				if(latitude >= -90.0 && latitude <= 90.0 && longitude >= -180.0 && longitude <= 180.0){
					current_position.setLatitude(latitude);
					current_position.setLongitude(longitude);
				} else{
					valid_position = false;
				}

				double altitudeMsl{std::stod(sentence_tokens.at(9))};

				bool valid_altitudes{true};

				// This should not be anything other than in meters, but just in case
				if(sentence_tokens.at(10) == "M" || sentence_tokens.at(10) == "m"){
					current_position.setMetersOverMsl(altitudeMsl);

				} else{
					valid_altitudes = false;
				}

				double altitudeEllipsoid{std::stod(sentence_tokens.at(9)) + std::stod(sentence_tokens.at(11))};

				// This should not be anything other than in meters, but just in case
				if(sentence_tokens.at(12) == "M" || sentence_tokens.at(12) == "m"){
					current_position.setMetersOverEllipsoid(altitudeEllipsoid);
				} else{
					valid_altitudes = false;
				}

				fix_quality.setFixQuality(
					gnss::FixQuality::FixType::valid,
					static_cast<unsigned int>(std::stoi(sentence_tokens.at(7))),
					std::stod(sentence_tokens.at(8)));


				if(valid_position && valid_altitudes && valid_utc_time){
					position = current_position;
					fix_quality.setFixQuality(
						gnss::FixQuality::FixType::valid,
						static_cast<unsigned int>(std::stoi(sentence_tokens.at(7))),
						std::stod(sentence_tokens.at(8)));

					return true;
				}

				return false;

			} else{
				return false; // Fixation not available!
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

	bool validateChecksum(std::string sentence){

		// Sentence should start with '$' or '!'.
		if(sentence.find_first_of("$!") != 0){
			return false;
		}

		// The data is between first character, checked above, and '*'.
		size_t sentenceEnd{sentence.find('*')};

		// There should always be an '*' between data and checksum
		if(sentenceEnd == std::string::npos){
			return false;
		}

		// Calculation of checksum
		unsigned char checksum{0};
		for(int pos{1}; pos < sentenceEnd; ++pos){
			checksum ^= sentence.at(pos);
		}

		// Compare calculated checksum against checksum at end of sentence (that is in hexadecimal notation)
		if(std::stoi(sentence.substr(sentenceEnd + 1, std::string::npos), nullptr, 16) == static_cast<int>(checksum)){
			return true;
		} else{
			return false;
		}

	}

	// Tokenize the sentence

	std::vector<std::string>  tokenize(std::string str, std::string separators){

		std::vector<std::string> tokens;
		size_t start{0}, end{str.find_first_of(separators, start)};

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

}