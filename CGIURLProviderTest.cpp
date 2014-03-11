/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <gtest/gtest.h>

#include "CGIURLProvider.h"

TEST(CGIURLProviderTest, GetURL) {
	CGIURLProvider provider("el-tramo.be", "/qr/?bla", "");	

	EXPECT_EQ("http://el-tramo.be/qr/?blo", provider.getURL("?blo"));
}

TEST(CGIURLProviderTest, GetURL_HTTPS) {
	CGIURLProvider provider("el-tramo.be", "/qr/?bla", "yes");	

	EXPECT_EQ("https://el-tramo.be/qr/?blo", provider.getURL("?blo"));
}

TEST(CGIURLProviderTest, GetURL_NoQuery) {
	CGIURLProvider provider("el-tramo.be", "/qr/", "");	

	EXPECT_EQ("http://el-tramo.be/qr/?blo", provider.getURL("?blo"));
}
