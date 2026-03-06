#pragma once

#include <string>
#include <vector>

namespace Corsairs::Utils::Strings {
    std::vector<std::string> SplitString(const std::string &s, char seperator);
	std::string ToLower(const std::string& value);
	std::string Replace(const std::string& value, unsigned char source, unsigned char dest);
}
