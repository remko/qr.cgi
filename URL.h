/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <string>
#include <map>

namespace URL {
	std::string encode(const std::string& url);
	std::string decode(const std::string& url);
	std::map<std::string, std::string> getQueryParameters(const std::string& query);
}
