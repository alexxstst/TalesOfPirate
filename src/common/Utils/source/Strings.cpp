#include <algorithm>
#include "Strings.h"

namespace Corsairs::Utils::Strings {

	std::vector<std::string> SplitString(const std::string &s, char seperator) {
		std::vector<std::string> output{};
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = s.find(seperator, pos)) != std::string::npos) {
			std::string substring(s.substr(prev_pos, pos - prev_pos));
			if (!substring.empty()) {
				output.emplace_back(substring);
			}

			prev_pos = ++pos;
		}

		const auto lastString = s.substr(prev_pos, pos - prev_pos);
		if (!lastString.empty())
			output.emplace_back(lastString); // Last word

		return output;
	}

	std::string ToLower(const std::string &value) {
		std::string data = value;
		std::transform(data.begin(), data.end(), data.begin(),
		               [](unsigned char c) { return std::tolower(c); });
		return data;
	}
	std::string Replace(const std::string &value, unsigned char source, unsigned char dest) {
		std::string data = value;
		std::transform(data.begin(), data.end(), data.begin(),
		               [source, dest](unsigned char c) {
			               if (c == source)
				               return dest;
			               return c;
		               });
		return data;
	}
}
