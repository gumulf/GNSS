#pragma once
#include <string>
#include <vector>
#include "gnss.h"

namespace gnss{

	bool isSentenceValid(std::string sentence);

	std::vector<std::string> tokenizeSentence(std::string sentence);

	// Parser


}