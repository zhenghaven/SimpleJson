// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>

#ifdef _MSC_VER
#include <windows.h>
#endif // _MSC_VER
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

GTEST_TEST(TestStringParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestStringParser, Construction)
{
	StringParser parser;
}

GTEST_TEST(TestStringParser, ParseCorrect)
{
	StringParser parser;

	// quote closing
	{
		std::string testInput = " \t \"\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// normal ASCII
	{
		std::string testInput = " \t \" !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// normal escapes
	{
		std::string testInput = " \t \"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "\" \\ / \b \f \n \r \t";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// \uXXXX escapes (UTF-16 pair)
	{
		std::string testInput = " \t \"\\ud83d\\uDE06\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "\xf0\x9f\x98\x86";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// \uXXXX escapes (UTF-16 single)
	{
		std::string testInput = " \t \"\\u6d4b\\u8bd5\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "\xe6\xb5\x8b\xe8\xaf\x95";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// \uXXXX escapes (ASCII)
	{
		std::string testInput = " \t \"\\u0054\\u0045\\u0053\\u0054\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "TEST";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

	// UTF-8
	{
		std::string testInput = " \t \"\xe6\xb5\x8b\xe8\xaf\x95\"   ";
		Internal::Obj::String res;
		Internal::Obj::String expRes = "\xe6\xb5\x8b\xe8\xaf\x95";
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, expRes);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, expRes);
	}

}

GTEST_TEST(TestStringParser, ParseError)
{
	StringParser parser;

	// Invalid inputs
	{
		std::string testInput = " \t \'   \'   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}
	{
		std::string testInput = " \t \" \" ;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}

	// normal escapes - invalid escape
	{
		std::string testInput = " \t \"\\X\"   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}
	{
		std::string testInput = " \t \"\\uXXXX\"   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}

	// \uXXXX escapes (UTF-16 pair) - broken pair
	{
		std::string testInput = " \t \"\\ud83d\"   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}
	{
		std::string testInput = " \t \"\\ud83d\\u0054\"   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}

	// UTF-8 - broken sequence
	{
		std::string testInput = " \t \"\xe6\xb5\x8b\xe8\x05\x05\"   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
	}
}
