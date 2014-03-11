/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <gtest/gtest.h>

#include "HTTPResponse.h"

TEST(HTTPResponseTest, ToString_VectorRValue) {
	std::vector<unsigned char> data {0x74, 0x72, 0x6f, 0x6e, 0xc3, 0xa7, 0x6f, 0x6e};
	
	EXPECT_EQ(
			"Status: 200 OK\r\n"
			"Content-Length: 8\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"tron\xc3\xa7on"
			"",
			HTTPResponse(HTTPResponse::OK, "text/html", std::move(data)).toString());
	// EXPECT_TRUE(data.empty());
}

TEST(HTTPResponseTest, ToString_String) {
	EXPECT_EQ(
			"Status: 200 OK\r\n"
			"Content-Length: 8\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"tron\xc3\xa7on"
			"",
			HTTPResponse(HTTPResponse::OK, "text/html", "tron\xc3\xa7on").toString());
}

TEST(HTTPResponseTest, ToString_UnsignedCharArray) {
	std::vector<unsigned char> data {0x74, 0x72, 0x6f, 0x6e, 0xc3, 0xa7, 0x6f, 0x6e};

	EXPECT_EQ(
			"Status: 200 OK\r\n"
			"Content-Length: 8\r\n"
			"Content-Type: text/html\r\n"
			"\r\n"
			"tron\xc3\xa7on"
			"",
			HTTPResponse(HTTPResponse::OK, "text/html", &data[0], data.size()).toString());
}
