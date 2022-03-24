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

GTEST_TEST(TestListParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestListParser, Construction)
{
	ListParserT<BoolParser> parser;
}

GTEST_TEST(TestListParser, ParseCorrect)
{
	using namespace Internal::Obj;

	ListParserT<BoolParser> parser;
	{
		std::string testInput = " \t [ true , false , true , true , false ]   ";
		ListT<Bool> res;
		ListT<Bool> exp = {
			Bool(true), Bool(false), Bool(true), Bool(true), Bool(false), };

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}
	{
		std::string testInput = " \t [true,false,true,true,false]   ";
		ListT<Bool> res;
		ListT<Bool> exp = {
			Bool(true), Bool(false), Bool(true), Bool(true), Bool(false), };

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}
	{
		std::string testInput = " \t [ ]   ";
		ListT<Bool> res;
		ListT<Bool> exp = {  };

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}
	{
		std::string testInput = " \t []   ";
		ListT<Bool> res;
		ListT<Bool> exp = {  };

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}

	{
		std::string testInput =
			" \r\n [ true , false , true , true , false ];   ";
		ListT<Bool> res;
		ListT<Bool> exp = {
			Bool(true), Bool(false), Bool(true), Bool(true), Bool(false), };

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
	}
}

GTEST_TEST(TestListParser, ParseError)
{
	ListParserT<BoolParser> parser;
	{
		std::string testInput = " \t [ true , false , true , true , false, ]   ";
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
		std::string testInput = " \t [ true , false , true , true , false,   ";
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
		std::string testInput = " \t [ true , false , true , true , false   ";
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
		std::string testInput = " \t [ true , false , true , true , false }  ";
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
		std::string testInput = " \t [ true , null , true , true , false ]   ";
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
		std::string testInput = " \t { true , false , true , true , false }   ";
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
		std::string testInput =
			" \r\n [ true , false , true , true , false ];   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}
