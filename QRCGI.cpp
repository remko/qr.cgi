/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "QRCGI.h"

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "URL.h"
#include "QRCodeGenerator.h"
#include "HTTPResponse.h"
#include "URLProvider.h"

extern const size_t favicon_ico_size;
extern const unsigned char favicon_ico[];

namespace MIMETypes {
	static const std::string TEXT_HTML = "text/html; charset=utf-8";
	static const std::string IMAGE_PNG = "image/png";
	static const std::string IMAGE_X_ICON = "image/x-icon";
}

static const std::string FAVICON_PATH = "?favicon";

QRCGI::QRCGI(URLProvider* urlProvider, QRCodeGenerator* qrGenerator) : 
		urlProvider(urlProvider), qrGenerator(qrGenerator) {
}

HTTPResponse QRCGI::getResponse(const std::string& queryString) {
	auto parameters = URL::getQueryParameters(queryString);
	auto query = parameters["q"];
	if (parameters.find("favicon") != parameters.end()) {
		return HTTPResponse(HTTPResponse::OK, MIMETypes::IMAGE_X_ICON, favicon_ico, favicon_ico_size);
	}
	else if (query.empty()) {
		auto bookmarkletURL = 
			std::string("javascript:void(window.open(")
			+ "'" + urlProvider->getURL("?q=") + "'+encodeURIComponent(location.href)," 
			+ "'QR Code','top=100,left=200,width=350,height=350,status=yes'));";

		std::ostringstream s;
		s << "<html>";
		s <<  "<head>";
		s <<		"<title>QR Code Generator</title>";
		s <<    "<link rel='shortcut icon' href='" << urlProvider->getURL(FAVICON_PATH) << "'/>";
		s <<  "</head>";
		s <<	"<body>";
		s <<    "Drag the following link into your bookmarks bar: ";
		s <<		"<a href=\"" << bookmarkletURL << "\"/>Create QR Code</a>";
		s <<	"</body>";
		s <<"</html>";

		return HTTPResponse(HTTPResponse::OK, MIMETypes::TEXT_HTML, s.str());
	}
	else {
		auto type = parameters["t"];
		if (type == "png") {
			return HTTPResponse(HTTPResponse::OK, MIMETypes::IMAGE_PNG, qrGenerator->generateQRPNG(query));
		}
		else {
			auto imageURL = urlProvider->getURL("?t=png&q=" + URL::encode(query));
			std::ostringstream s;
			s << "<html>";
			s <<  "<head>";
			s <<		"<title>QR Code</title>";
			s <<    "<link rel='shortcut icon' href='" << urlProvider->getURL(FAVICON_PATH) << "'/>";
			s <<  "</head>";
			s <<	"<body>";
			s <<    "<div style='width: 100%; height: 100%;'>";
			s <<		  "<img style='max-width: 100%; max-height:100%; display: block; margin: 0 auto;' src=\"" << imageURL << "\"/>";
			s <<    "</div>";
			s <<	"</body>";
			s <<"</html>";

			return HTTPResponse(HTTPResponse::OK, MIMETypes::TEXT_HTML, s.str());
		}
	}
}
