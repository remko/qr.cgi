/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#pragma once

#include <vector>
#include <string>
#include <ostream>

class HTTPResponse {
	public:
		enum Status {
			OK,
			NOT_FOUND
		};

		HTTPResponse(Status status, const std::string& mimeType, const std::vector<unsigned char>&& content);
		HTTPResponse(Status status, const std::string& mimeType, const unsigned char* content, size_t contentSize);
		HTTPResponse(Status status, const std::string& mimeType, const std::string& content);

		std::string toString() const;

	private:
		static std::string getStatusString(Status status);
		friend std::ostream& operator<<(std::ostream&, const HTTPResponse&);

	private:
		struct HTTPHeader {
			HTTPHeader(const std::string& name, const std::string& value) : name(name), value(value) {}
			std::string name;
			std::string value;
		};

		Status status;
		std::vector<unsigned char> content;
		std::vector<HTTPHeader> headers;
};

std::ostream& operator<<(std::ostream&, const HTTPResponse&);
