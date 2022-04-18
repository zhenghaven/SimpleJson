// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>

#include <SimpleJson/SimpleJson.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestNullParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestNullParser, Construction)
{
	NullParser parser;
}

GTEST_TEST(TestNullParser, ParseCorrect)
{
	NullParser parser;
	{
		std::string testInput = " \t null   ";
		EXPECT_NO_THROW(
			parser.Parse(testInput);
		);
		EXPECT_NO_THROW(
			parser.ParseTillEnd(testInput);
		);
	}

	{
		std::string testInput = " \r\n null;   ";
		EXPECT_NO_THROW(
			parser.Parse(testInput);
		);
	}
}

GTEST_TEST(TestNullParser, ParseError)
{
	NullParser parser;
	{
		std::string testInput = " \t n ull   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}

	{
		std::string testInput = " \t nu ll   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}

	{
		std::string testInput = " \t , null   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}

	{
		std::string testInput = " \r\n null;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}
