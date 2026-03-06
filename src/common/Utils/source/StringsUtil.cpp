#include "StringsUtil.h"

#include <vector>


std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
}

std::vector<std::string_view> SplitString(std::string_view s, char separator) {
	std::vector<std::string_view> output{};
	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(separator, pos)) != std::string::npos) {
		std::string_view substring(s.substr(prev_pos, pos - prev_pos));
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

std::vector<std::string_view> SplitString(std::string_view s, const std::int32_t maxCount, char separator) {
	std::int32_t idx{};
	std::vector<std::string_view> output{};
	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(separator, pos)) != std::string::npos && idx < maxCount) {
		std::string_view substring(s.substr(prev_pos, pos - prev_pos));
		const_cast<char*>(s.data())[pos] = 0;
		if (!substring.empty()) {
			output.emplace_back(substring);
		}

		prev_pos = ++pos;
		++idx;
	}

	if (idx < maxCount) {
		const auto lastString = s.substr(prev_pos, pos - prev_pos);
		if (!lastString.empty())
			output.emplace_back(lastString); // Last word
	}

	return output;
}
