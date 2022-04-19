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

GTEST_TEST(TestDictParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestDictParser, Construction)
{
	DictParserT<BoolParser> parser;
}

GTEST_TEST(TestDictParser, ParseCorrect)
{
	using namespace Internal::Obj;

	DictParserT<BoolParser> parser;
	{
		std::string testInput =
			" \t { \"0\" : false , \"1\" : true , \"2\" : false }   ";
		DictT<HashableObject, Bool> res;
		DictT<HashableObject, Bool> exp = {
			{HashableObject(String("0")), Bool(false)},
			{HashableObject(String("1")), Bool(true)},
			{HashableObject(String("2")), Bool(false)},
		};

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
			" \t {\"0\":false,\"1\":true,\"2\":false}   ";
		DictT<HashableObject, Bool> res;
		DictT<HashableObject, Bool> exp = {
			{HashableObject(String("0")), Bool(false)},
			{HashableObject(String("1")), Bool(true)},
			{HashableObject(String("2")), Bool(false)},
		};

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
			" \t { }   ";
		DictT<HashableObject, Bool> res;
		DictT<HashableObject, Bool> exp = { };

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
			" \t {}   ";
		DictT<HashableObject, Bool> res;
		DictT<HashableObject, Bool> exp = { };

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
			" \t { \"0\" : false , \"1\" : true , \"2\" : false };   ";
		DictT<HashableObject, Bool> res;
		DictT<HashableObject, Bool> exp = {
			{HashableObject(String("0")), Bool(false)},
			{HashableObject(String("1")), Bool(true)},
			{HashableObject(String("2")), Bool(false)},
		};

		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
	}
}

GTEST_TEST(TestDictParser, ParseError)
{
	DictParserT<BoolParser> parser;
	{
		std::string testInput =
			" \t { \"0\" : false , \"1\" : true , \"2\" : false, }   ";
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
			" \t { \"0\" : false , \"1\" : true , \"2\" : false,    ";
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
			" \t { \"0\" : false , \"1\" : true , \"2\" : false    ";
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
			" \t { \"0\" : false , \"1\" : true , \"2\" : false ]   ";
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
			" \t [ \"0\" : false , \"1\" : true , \"2\" : false ]   ";
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
			" \t { \"0\" : false , \"1\" : null , \"2\" : false }   ";
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
			" \t { \"0\" : false , \"1\" ; true , \"2\" : false }   ";
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
			" \t { \"0\" : false , \"1\" : true , \"2\" : false };   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}

GTEST_TEST(TestDictParser, DictKeyParser)
{
	DictKeyParser parser;

	// normal escapes
	{
		std::string testInput = " \t \"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"   ";
		Internal::Obj::HashableObject res;
		Internal::Obj::HashableObject expRes =
			Internal::Obj::String("\" \\ / \b \f \n \r \t");
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
		Internal::Obj::HashableObject res;
		Internal::Obj::HashableObject expRes =
			Internal::Obj::String("\xf0\x9f\x98\x86");
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
