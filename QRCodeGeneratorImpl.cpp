/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include "QRCodeGeneratorImpl.h"

#include <cassert>
#include <array>
#include <memory>
#include <qrencode.h>
#include <png.h>
#include <iostream>

static const int size = 10;
static const int MARGIN = 1;

static std::vector<unsigned char> convertToPNG(QRcode* qrcode) {
	auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	assert(png_ptr);
	auto info_ptr = png_create_info_struct(png_ptr);
	assert(info_ptr);

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr); std::cerr << "Error creating PNG image" << std::endl;
		return std::vector<unsigned char>();
	}

	std::array<png_color, 2> palette {{
		{ 0, 0, 0 },
		{ 255, 255, 255 }
	}};
	png_set_PLTE(png_ptr, info_ptr, &palette[0], 2);

	std::vector<unsigned char> result;
	png_set_write_fn(png_ptr, &result, [](png_structp png_ptr, png_bytep out, png_size_t size) {
		auto& data = *(std::vector<unsigned char>*) png_get_io_ptr(png_ptr);
		data.insert(data.end(), out, out + size);
	}, [](png_structp) {});

	auto width = (qrcode->width + MARGIN*2) * size;
	png_set_IHDR(png_ptr, info_ptr, width, width, 1,
			PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	std::vector<unsigned char> row((width + 7) / 8, ~0);
	for (int i = 0; i < MARGIN*size; ++i) {
		png_write_row(png_ptr, &row[0]);
	}

	auto p = qrcode->data;
	for(int y = 0; y < qrcode->width; ++y) {
		std::fill(row.begin(), row.end(), ~0);
		auto q = &row[0] + (MARGIN*size / 8);
		auto currentBit = 7 - (MARGIN*size % 8);
		for (auto x = 0; x < qrcode->width; ++x) {
			for (auto i = 0; i < size; ++i) {
				*q ^= (*p & 1) << currentBit;
				if (currentBit == 0) {
					q++;
					currentBit = 7;
				}
				else {
					currentBit--;
				}
			}
			++p;
		}
		for (auto i = 0; i < size; ++i) {
			png_write_row(png_ptr, &row[0]);
		}
	}

	std::fill(row.begin(), row.end(), ~0);
	for (int i = 0; i < MARGIN*size; ++i) {
		png_write_row(png_ptr, &row[0]);
	}

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	return result;
}



std::vector<unsigned char> QRCodeGeneratorImpl::generateQRPNG(const std::string& data) {
	auto qrCode = std::shared_ptr<QRcode>(QRcode_encodeData(data.length(), (const unsigned char*) data.c_str(), 0, QR_ECLEVEL_L), QRcode_free);
	return convertToPNG(qrCode.get());
}
