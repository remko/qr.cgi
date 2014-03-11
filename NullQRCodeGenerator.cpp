/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "NullQRCodeGenerator.h"

std::vector<unsigned char> NullQRCodeGenerator::generateQRPNG(const std::string& data) {
	return std::vector<unsigned char>(data.begin(), data.end());
}
