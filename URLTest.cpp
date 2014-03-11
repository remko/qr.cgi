/*
 * Copyright (c) 2014 Remko Tronçon
 * Licensed under the GNU General Public License.
 * See the COPYING file for more information.
 */

#include <gtest/gtest.h>

#include "URL.h"

TEST(URLTest, Encode_NoSpecialCharacters) {
	EXPECT_EQ("foobar123", URL::encode("foobar123"));
}

TEST(URLTest, Encode_Spaces) {
	EXPECT_EQ("foo+bar+baz", URL::encode("foo bar baz"));
}

TEST(URLTest, Encode_StrangeCharacters) {
	EXPECT_EQ("foo%2bbar%24baz", URL::encode("foo+bar$baz"));
}

TEST(URLTest, Decode_NoSpecialCharacters) {
	EXPECT_EQ("foobar123", URL::decode("foobar123"));
}

TEST(URLTest, Decode_Spaces) {
	EXPECT_EQ("foo bar baz", URL::decode("foo+bar+baz"));
}

TEST(URLTest, Decode_StrangeCharacters) {
	EXPECT_EQ("foo+bar$baz", URL::decode("foo%2bbar%24baz"));
}

TEST(URLTest, Decode_CharacterWithUpperBitSet) {
	EXPECT_EQ("tron\xc3\xa7on", URL::decode("tron%c3%a7on"));
}

TEST(URLTest, Decode_EscapeAtEnd) {
	EXPECT_EQ("foo$", URL::decode("foo%24"));
}

TEST(URLTest, Decode_SpaceAtEnd) {
	EXPECT_EQ("foo ", URL::decode("foo+"));
}

TEST(URLTest, Decode_UnfinishedEscape) {
	EXPECT_EQ("foo ", URL::decode("foo+"));
}

TEST(URLTest, GetQueryParameters) {
	auto result = URL::getQueryParameters("foo=bar&baz=bam");

	EXPECT_EQ(2, result.size());
	EXPECT_EQ("bar", result["foo"]);
	EXPECT_EQ("bam", result["baz"]);
}

TEST(URLTest, GetQueryParameters_OneParameter) {
	auto result = URL::getQueryParameters("foo=bar");

	EXPECT_EQ(1, result.size());
	EXPECT_EQ("bar", result["foo"]);
}

TEST(URLTest, GetQueryParameters_EmptyQuery) {
	auto result = URL::getQueryParameters("");

	EXPECT_TRUE(result.empty());
}

TEST(URLTest, GetQueryParameters_SpecialCharacters) {
	auto result = URL::getQueryParameters("foo+foo=bar%20bar&baz=tron%c3%a7on");

	EXPECT_EQ(2, result.size());
	EXPECT_EQ("bar bar", result["foo foo"]);
	EXPECT_EQ("tron\xc3\xa7on", result["baz"]);
}

TEST(URLTest, GetQueryParameters_EmptyLast) {
	auto result = URL::getQueryParameters("foo=bar&baz=bam&");

	EXPECT_EQ(2, result.size());
	EXPECT_EQ("bar", result["foo"]);
	EXPECT_EQ("bam", result["baz"]);
}

TEST(URLTest, GetQueryParameters_EmptyMiddle) {
	auto result = URL::getQueryParameters("foo=bar&&baz=bam");

	EXPECT_EQ(2, result.size());
	EXPECT_EQ("bar", result["foo"]);
	EXPECT_EQ("bam", result["baz"]);
}

TEST(URLTest, GetQueryParameters_NoValues) {
	auto result = URL::getQueryParameters("foo&bar");

	EXPECT_EQ(2, result.size());
	EXPECT_TRUE(result.find("foo") != result.end());
	EXPECT_TRUE(result["foo"].empty());
	EXPECT_TRUE(result.find("bar") != result.end());
	EXPECT_TRUE(result["bar"].empty());
}
