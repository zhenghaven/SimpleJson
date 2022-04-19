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

GTEST_TEST(TestBoolParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestBoolParser, Construction)
{
	BoolParser parser;
}

GTEST_TEST(TestBoolParser, ParseCorrect)
{
	BoolParser parser;
	{
		std::string testInput = " \t true   ";
		Internal::Obj::Bool res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(true));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(true));
	}
	{
		std::string testInput = " \t false   ";
		Internal::Obj::Bool res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(false));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(false));
	}

	{
		std::string testInput = " \r\n true;   ";
		Internal::Obj::Bool res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(true));
	}
	{
		std::string testInput = " \r\n false ;   ";
		Internal::Obj::Bool res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Bool(false));
	}
}

GTEST_TEST(TestBoolParser, ParseError)
{
	BoolParser parser;
	{
		std::string testInput = " \t t rue   ";
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
		std::string testInput = " \t fa lse   ";
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
		std::string testInput = " \t , true   ";
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
		std::string testInput = " \r\n false;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}
