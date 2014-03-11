/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#pragma once

#include <string>

class URLProvider {
	public:
		virtual ~URLProvider();

		virtual std::string getURL(const std::string& path) const = 0;
};
