/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#pragma once

#include <string>

#include "HTTPResponse.h"

class URLProvider;
class QRCodeGenerator;

class QRCGI {
	public:
		QRCGI(URLProvider*, QRCodeGenerator*);

		HTTPResponse getResponse(const std::string& query);

	private:
		URLProvider* urlProvider;
		QRCodeGenerator* qrGenerator;
};
