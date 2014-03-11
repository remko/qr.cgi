/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "CGIURLProvider.h"


CGIURLProvider::CGIURLProvider(
		const std::string& httpHost, 
		const std::string& requestURI,
		const std::string& https) {
	auto scriptPath = requestURI;
	auto queryStart = scriptPath.find("?");
	if (queryStart != std::string::npos) {
		scriptPath = scriptPath.substr(0, queryStart);
	}
	baseURL = (https.empty() ? "http://" : "https://") + httpHost + scriptPath;
}

std::string CGIURLProvider::getURL(const std::string& path) const {
	return baseURL + path;
}
