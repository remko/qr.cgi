/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <iostream>

#include "QRCodeGeneratorImpl.h"
#include "HTTPResponse.h"
#include "CGIURLProvider.h"
#include "QRCGI.h"

std::string getEnv(const char* var) {
	auto rawValue = ::getenv(var);
	return rawValue ? std::string(rawValue) : "";
}

int main(int, char**, char**) {
	// std::ostringstream s2;
	// char** env;
	// for (env = envp; *env != 0; env++) {
	// 	s2 << *env << std::endl;
	// }

	CGIURLProvider urlProvider(getEnv("HTTP_HOST"), getEnv("REQUEST_URI"), getEnv("HTTPS"));
	QRCodeGeneratorImpl qrGenerator;
	std::cout << QRCGI(&urlProvider, &qrGenerator).getResponse(getEnv("QUERY_STRING"));
	return 0;
}
