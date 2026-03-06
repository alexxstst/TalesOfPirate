#pragma once

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cctype>
#include <locale>

std::string ReplaceString(std::string subject, const std::string& search,
						  const std::string& replace);

void ReplaceStringInPlace(std::string& subject, const std::string& search,
						  const std::string& replace);

std::vector<std::string_view> SplitString(std::string_view s, char separator);
std::vector<std::string_view> SplitString(std::string_view s, const std::int32_t maxCount, char separator);

static inline void LTrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void RTrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
				return !std::isspace(ch);
			}).base(),
			s.end());
}

// trim from both ends (in place)
static inline void Trim(std::string& s) {
	RTrim(s);
	LTrim(s);
}
