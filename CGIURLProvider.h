/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#pragma once

#include "URLProvider.h"

class CGIURLProvider : public URLProvider {
	public:
		CGIURLProvider(
				const std::string& httpHost,
				const std::string& requestURI,
				const std::string& https);

		virtual std::string getURL(const std::string& path) const;

	private:
		std::string baseURL;
};
