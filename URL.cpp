/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "URL.h"

#include <sstream>
#include <iomanip>

std::string URL::encode(const std::string& url) {
	std::ostringstream result;
	result << std::hex << std::setfill('0');
	for (auto c : url) {
		if (isalnum(c) || c == '-' || c == '_') {
			result << c;
		}
		else if (c == ' ') {
			result << '+';
		}
		else {
			result << '%' << std::setw(2) << (int) c;
		}
	}
	return result.str();
}

std::string URL::decode(const std::string& url) {
	std::ostringstream result;
	for (size_t i = 0; i < url.size(); ++i) {
		if (url[i] == '%') {
			if (i + 2 < url.size()) {
				std::stringstream s;
				s << std::hex << url.substr(i+1, 2);
				unsigned int value;
				s >> value;
				if (s.fail() || s.bad()) {
					return "";
				}
				result << static_cast<char>(value);
				i += 2;
			}
			else {
				return "";
			}
		}
		else if (url[i] == '+') {
			result << ' ';
		}
		else {
			result << url[i];
		}
	}
	return result.str();
}

static std::pair<std::string, std::string> parseParameter(const std::string& query, size_t start, size_t end) {
	auto equalsIndex = query.find('=', start);
	if (equalsIndex != std::string::npos && equalsIndex < end) {
		auto key = URL::decode(query.substr(start, equalsIndex - start));
		auto value = URL::decode(query.substr(equalsIndex+1, end - equalsIndex - 1));
		return std::make_pair(key, value);
	}
	else {
		return std::make_pair(query.substr(start, end - start), "");
	}
}

std::map<std::string, std::string> URL::getQueryParameters(const std::string& query) {
	std::map<std::string, std::string> result;
	size_t parameterStart = 0;
	auto parameterEnd = std::string::npos;
	while ((parameterEnd = query.find('&', parameterStart)) != std::string::npos) {
		if (parameterStart < parameterEnd) {
			result.insert(parseParameter(query, parameterStart, parameterEnd));
		}
		parameterStart = parameterEnd + 1;
	}
	if (parameterStart < query.size()) {
		result.insert(parseParameter(query, parameterStart, std::string::npos));
	}
	return result;
}
