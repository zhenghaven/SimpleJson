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

GTEST_TEST(TestGenericParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestGenericParser, Construction)
{
	GenericObjectParser parser;
}

namespace
{

static const char gsk_testInput_01[] =
"{\
	\"NullKey\" : null,\n\
	\"TrueKey\" : true,\n\
	\"FalseKey\" : false,\n\
	\"IntKey\" : 1234567890,\n\
	\"RealKey\" : 123456789.0,\n\
	\"StrKey\" : \" !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\",\n\
	\"ListKey\" : [ null, true, 123456, 123.0, \"abcdef\" ],\n\
	\"DictKey\" : {\n\
		\"0\" : null,\n\
		\"1\" : true,\n\
		\"2\" : 2,\n\
		\"3\" : 3.0\n\
	}\n\
}";

static Internal::Obj::Object GetExpRes_01()
{
	using namespace Internal::Obj;
	return Object(Dict({
		{ HashableObject(String("NullKey")),  Object(Null()) },
		{ HashableObject(String("TrueKey")),  Object(Bool(true)) },
		{ HashableObject(String("FalseKey")), Object(Bool(false)) },
		{ HashableObject(String("IntKey")),   Object(Int64(1234567890LL)) },
		{ HashableObject(String("RealKey")),  Object(Double(123456789.0)) },
		{ HashableObject(String("StrKey")),   Object(String(" !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}")) },
		{ HashableObject(String("ListKey")),  Object(List({
			Object(Null()),
			Object(Bool(true)),
			Object(Int64(123456LL)),
			Object(Double(123.0)),
			Object(String("abcdef")),
			})) },
		{ HashableObject(String("DictKey")),  Object(Dict({
			{ HashableObject(String("0")), Object(Null()) },
			{ HashableObject(String("1")), Object(Bool(true)) },
			{ HashableObject(String("2")), Object(Int64(2LL)) },
			{ HashableObject(String("3")), Object(Double(3.0)) },
			})) },
	}));
}

} // namespace


GTEST_TEST(TestGenericParser, ParseCorrect)
{
	GenericObjectParser parser;

	{
		Internal::Obj::Object res;
		Internal::Obj::Object exp = GetExpRes_01();
		EXPECT_NO_THROW(
			res = parser.Parse(gsk_testInput_01);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(gsk_testInput_01);
		);
		EXPECT_EQ(res, exp);
	}
}

GTEST_TEST(TestGenericParser, InternalListParseCorrect)
{
	using _RetType = Internal::Obj::ListT<Internal::Obj::Object>;
	ListParserImpl<
		std::string,
		GenericObjectParser,
		_RetType > parser;

	{
		std::string testInput = "[ 123, \"value2\" ]";
		_RetType res;
		_RetType exp = {
			Internal::Obj::Int64(123LL),
			Internal::Obj::String("value2"), };
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}
}

GTEST_TEST(TestGenericParser, InternalDictParseCorrect)
{
	using DictKeyParser =
		StringParserImpl<std::string,
		Internal::Obj::String,
		Internal::Obj::HashableObject>;

	using _RetType = Internal::Obj::DictT<
		Internal::Obj::HashableObject, Internal::Obj::Object>;

	DictParserImpl<
		std::string,
		DictKeyParser,
		GenericObjectParser,
		_RetType > parser;

	{
		std::string testInput = "{ \"key1\" : 123, \"key2\" : \"value2\" }";
		_RetType res;
		_RetType exp = {
			{Internal::Obj::String("key1"), Internal::Obj::Int64(123LL)},
			{Internal::Obj::String("key2"), Internal::Obj::String("value2")}, };
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, exp);
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, exp);
	}
}

GTEST_TEST(TestGenericParser, ParseError)
{
	GenericObjectParser parser;

	{
		std::string testInput = " [+1]   ";
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
		std::string testInput = " /   ";
		EXPECT_THROW(
			parser.Parse(testInput);,
			ParseError
		);
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}
