/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <gtest/gtest.h>
#include <memory>

#include "QRCGI.h"
#include "CGIURLProvider.h"
#include "NullQRCodeGenerator.h"

class QRCGITest : public ::testing::Test {
	protected:
		QRCGITest() : urlProvider("el-tramo.be", "/qr/", "") {}

		std::unique_ptr<QRCGI> create() {
			return std::unique_ptr<QRCGI>(new QRCGI(&urlProvider, &qrGenerator));
		}

		CGIURLProvider urlProvider;
		NullQRCodeGenerator qrGenerator;
};

TEST_F(QRCGITest, GetResponse_Empty) {
	auto response = create()->getResponse("").toString();
  auto expected = std::string() 
		+ "Status: 200 OK\r\n" 
		+ "Content-Length: 368\r\n"
		+ "Content-Type: text/html; charset=utf-8\r\n" 
		+ "\r\n"
		+ "<html><head><title>QR Code Generator</title><link rel='shortcut icon' href='http://el-tramo.be/qr/?favicon'/></head><body>Drag the following link into your bookmarks bar: <a href=\"javascript:void(window.open('http://el-tramo.be/qr/?q='+encodeURIComponent(location.href),'QR Code','top=100,left=200,width=350,height=350,status=yes'));\"/>Create QR Code</a></body></html>";

	EXPECT_EQ(expected, response);
}

TEST_F(QRCGITest, GetResponse_WithQuery) {
	auto response = create()->getResponse("q=abc+def").toString();
  auto expected = std::string() 
		+ "Status: 200 OK\r\n" 
		+ "Content-Length: 297\r\n"
		+ "Content-Type: text/html; charset=utf-8\r\n" 
		+ "\r\n"
		+ "<html><head><title>QR Code</title><link rel='shortcut icon' href='http://el-tramo.be/qr/?favicon'/></head><body><div style='width: 100%; height: 100%;'><img style='max-width: 100%; max-height:100%; display: block; margin: 0 auto;' src=\"http://el-tramo.be/qr/?t=png&q=abc+def\"/></div></body></html>";

	EXPECT_EQ(expected, response);
}

TEST_F(QRCGITest, GetResponse_WithQueryAndType) {
	auto response = create()->getResponse("q=abc+def&t=png").toString();
  auto expected = std::string() 
		+ "Status: 200 OK\r\n" 
		+ "Content-Length: 7\r\n"
		+ "Content-Type: image/png\r\n"
		+ "\r\n"
		+ "abc def";

	EXPECT_EQ(expected, response);
}

TEST_F(QRCGITest, GetResponse_WithFavIcon) {
	auto response = create()->getResponse("favicon").toString();

	EXPECT_TRUE(response.find("Status: 200") != std::string::npos);
	EXPECT_TRUE(response.find("Content-Type: image/x-icon") != std::string::npos);
}
