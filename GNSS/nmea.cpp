#include "nmea.h"


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

	std::vector<std::string> tokenize(std::string str, char separator){
		return std::vector<std::string>{""};

	}

	// Parser

}