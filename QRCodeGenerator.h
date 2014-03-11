/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <vector>
#include <string>

class QRCodeGenerator {
	public:
		virtual ~QRCodeGenerator();

		virtual std::vector<unsigned char> generateQRPNG(const std::string& data) = 0;
};
