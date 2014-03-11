/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "HTTPResponse.h"

#include <sstream>
#include <cassert>
#include <iterator>
#include <algorithm>

HTTPResponse::HTTPResponse(
		Status status, 
		const std::string& mimeType, 
		const std::vector<unsigned char>&& content) :
			status(status), content(content) {
	headers.push_back(HTTPHeader("Status", getStatusString(status)));
	headers.push_back(HTTPHeader("Content-Length", std::to_string(this->content.size())));
	headers.push_back(HTTPHeader("Content-Type", mimeType));
}

HTTPResponse::HTTPResponse(
		Status status, 
		const std::string& mimeType, 
		const unsigned char* content, size_t contentSize) :
			status(status), content(content, content + contentSize) {
	headers.push_back(HTTPHeader("Status", getStatusString(status)));
	headers.push_back(HTTPHeader("Content-Length", std::to_string(this->content.size())));
	headers.push_back(HTTPHeader("Content-Type", mimeType));
}

HTTPResponse::HTTPResponse(
		Status status, 
		const std::string& mimeType, 
		const std::string& content) :
			status(status), content(content.begin(), content.end()) {
	headers.push_back(HTTPHeader("Status", getStatusString(status)));
	headers.push_back(HTTPHeader("Content-Length", std::to_string(this->content.size())));
	headers.push_back(HTTPHeader("Content-Type", mimeType));
}

std::string HTTPResponse::getStatusString(Status status) {
	switch (status) {
		case OK: return "200 OK";
		case NOT_FOUND: return "404 Not Found";
	}
	assert(false);
	return "";
}

std::ostream& operator<<(std::ostream& os, const HTTPResponse& response) {
	for (auto& header : response.headers) {
		os << header.name << ": " << header.value << "\r\n";
	}
	os << "\r\n";
	std::ostream_iterator<unsigned char> out(os);
	std::copy(response.content.begin(), response.content.end(), out);
	return os;
}

std::string HTTPResponse::toString() const {
	std::ostringstream s;
	s << *this;
	return s.str();
}
