/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "QRCodeGenerator.h"

class QRCodeGeneratorImpl : public QRCodeGenerator {
	public:
		virtual std::vector<unsigned char> generateQRPNG(const std::string& data) override;
};
