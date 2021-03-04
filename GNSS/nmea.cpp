#include "nmea.h"
#include <cmath>

#include <iostream> // TODO Remove after initial testing and developemnt

namespace gnss{

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

	std::vector<std::string> tokenize(std::string str, std::string separators){
		
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

	// Parser

	void nmeaParser(std::vector<std::string> sentence){

		std::string sentenceType{sentence.front().substr(3, 3)};

		std::cout << "The sentence type is: " << sentenceType << "\n";

		if(sentenceType == "GGA"){
			if(stoi(sentence.at(6)) >= 1){

				std::cout << "Time: " << sentence.at(1).substr(0, 2) << ":"
					<< sentence.at(1).substr(2, 2) << ":" << sentence.at(1).substr(4, 2)
					//<< "." << sentence.at(1).substr(7, std::string::npos)
					<< " UTC\n";

				std::cout << "Position: "
					<< sentence.at(2).substr(0, 2) << "\370"
					<< sentence.at(2).substr(2, 2) << "'"
					<< static_cast<int>(round(stod("0" + sentence.at(2).substr(4, std::string::npos)) * 60)) << "\" "
					<< ((sentence.at(3) == "N") ? "North" : "South")
					<< ", "
					<< sentence.at(4).substr(0, 3) << "\370"
					<< sentence.at(4).substr(3, 2) << "'"
					<< static_cast<int>(round(stod("0" + sentence.at(4).substr(5, std::string::npos)) * 60)) << "\" "
					<< ((sentence.at(5) == "E") ? "East" : "West")
					<< "\n";

				std::cout << "Altitude: "
					<< sentence.at(9) << " " << (sentence.at(10) == "M" ? "meters" : sentence.at(10)) << " above MSL" 
					<< ", "
					<< (stod(sentence.at(9)) + stod(sentence.at(11))) << " " << (sentence.at(12) == "M" ? "meters" : sentence.at(12)) << " above ellipsoid"
					<< "\n";
			
				std::cout << "Satelites used: "
					<< sentence.at(7)
					<< "\n";

				std::cout << "HDOP: "
					<< sentence.at(8)
					<< "\n";

			} else{
				std::cout << "Fix not available!\n";
			}

		} else{
			std::cout << "Parsing of type " << sentence.at(0) << " in not implemented!\n";

			// Unkown sentence type
			// TODO Do good stuff here
		}
		
	}


}