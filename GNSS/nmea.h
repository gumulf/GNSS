#pragma once
#include <string>
#include <vector>
#include "gnss.h"

// http://freenmea.net/docs

namespace gnss{

	bool validateChecksum(std::string sentence);

	std::vector<std::string> tokenize(std::string str, std::string separators);

	// Parser


}