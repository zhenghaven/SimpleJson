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

GTEST_TEST(TestJsonWriter, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestJsonWriter, NullWriter)
{
	{
		std::string res;
		WriterConfig cfg;
		WriterStates st;

		JsonWriterNull::Write(std::back_inserter(res), cfg, st);
		EXPECT_EQ(res, "null");
	}
}

GTEST_TEST(TestJsonWriter, BoolWriter)
{
	{
		std::string res;
		WriterConfig cfg;
		WriterStates st;

		JsonWriterRealNum::Write(
			std::back_inserter(res),
			Internal::Obj::Bool(true),
			cfg, st);
		EXPECT_EQ(res, "true");

		res = "";

		JsonWriterRealNum::Write(
			std::back_inserter(res),
			Internal::Obj::Bool(false),
			cfg, st);
		EXPECT_EQ(res, "false");
	}
}

GTEST_TEST(TestJsonWriter, NumWriter)
{
	{
		std::string res;
		WriterConfig cfg;

		JsonWriterRealNum::Write(
			std::back_inserter(res),
			Internal::Obj::Int64(1234567890LL),
			cfg, WriterStates());
		EXPECT_EQ(res, "1234567890");

		res = "";

		JsonWriterRealNum::Write(
			std::back_inserter(res),
			Internal::Obj::Double(123.0),
			cfg, WriterStates());
		EXPECT_EQ(res, "123.0");

		res = "";

		JsonWriterRealNum::Write(
			std::back_inserter(res),
			Internal::Obj::Double(0.0),
			cfg, WriterStates());
		EXPECT_EQ(res, "0.0");
	}
}

GTEST_TEST(TestJsonWriter, StringWriter)
{
	{
		std::string res;
		WriterConfig cfg;
		WriterStates st;

		JsonWriterString::Write(
			std::back_inserter(res),
			Internal::Obj::String(
				" !\"#$%&\'()*+,-./0123456789:;<=>?@"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
				"abcdefghijklmnopqrstuvwxyz{|}"),
			cfg, st);
		EXPECT_EQ(
			res,
			"\" !\\\"#$%&'()*+,-.\\/0123456789:;<=>?@"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`"
			"abcdefghijklmnopqrstuvwxyz{|}\"");

		res = "";

		JsonWriterString::Write(
			std::back_inserter(res),
			Internal::Obj::String("\" \\ / \b \f \n \r \t"),
			cfg, st);
		EXPECT_EQ(res, "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");

		res = "";

		JsonWriterString::Write(
			std::back_inserter(res),
			Internal::Obj::String("\xf0\x9f\x98\x86"),
			cfg, st);
		EXPECT_EQ(res, "\"\\uD83D\\uDE06\"");

		res = "";

		JsonWriterString::Write(
			std::back_inserter(res),
			Internal::Obj::String("\xe6\xb5\x8b\xe8\xaf\x95"),
			cfg, st);
		EXPECT_EQ(res, "\"\\u6D4B\\u8BD5\"");
	}
}

GTEST_TEST(TestJsonWriter, UnserializableType)
{
	{
		Internal::Obj::Dict dict1({
			{Internal::Obj::Bytes({ 0x20U, 0x21U, }), Internal::Obj::String("TestVal")}
		});

		EXPECT_THROW(DumpStr(dict1), SerializeTypeError);
	}

	{
		Internal::Obj::Dict dict1({
			{Internal::Obj::String("TestKey"), Internal::Obj::Bytes({ 0x20U, 0x21U, })}
		});

		EXPECT_THROW(DumpStr(dict1), SerializeTypeError);
	}
}

namespace
{
static constexpr char sk_listTestInput_01[] =
"[\n\
	[\n\
		0,\n\
		1,\n\
		[\n\
			0,\n\
			1,\n\
			2,\n\
			3,\n\
			4,\n\
			5\n\
		],\n\
		[],\n\
		[\n\
			0\n\
		]\n\
	]\n\
]";

static constexpr char sk_listTestInput_02[] =
"[[0,1,[0,1,2,3,4,5],[],[0]]]";

static constexpr char sk_dictTestInput_01_a[] =
"{\n\
	\"a\" : {},\n\
	\"b\" : {\n\
		\"c\" : [\n\
			0,\n\
			1,\n\
			2\n\
		],\n\
		\"e\" : {\n\
			\"a\" : 0\n\
		}\n\
	}\n\
}";
static constexpr char sk_dictTestInput_01_b[] =
"{\n\
	\"b\" : {\n\
		\"c\" : [\n\
			0,\n\
			1,\n\
			2\n\
		],\n\
		\"e\" : {\n\
			\"a\" : 0\n\
		}\n\
	},\n\
	\"a\" : {}\n\
}";
static constexpr char sk_dictTestInput_01_c[] =
"{\n\
	\"a\" : {},\n\
	\"b\" : {\n\
		\"e\" : {\n\
			\"a\" : 0\n\
		},\n\
		\"c\" : [\n\
			0,\n\
			1,\n\
			2\n\
		]\n\
	}\n\
}";
static constexpr char sk_dictTestInput_01_d[] =
"{\n\
	\"b\" : {\n\
		\"e\" : {\n\
			\"a\" : 0\n\
		},\n\
		\"c\" : [\n\
			0,\n\
			1,\n\
			2\n\
		]\n\
	},\n\
	\"a\" : {}\n\
}";

static constexpr char sk_dictTestInput_02_a[] =
"{\"a\":{},\"b\":{\"c\":[0,1,2],\"e\":{\"a\":0}}}";
static constexpr char sk_dictTestInput_02_b[] =
"{\"b\":{\"c\":[0,1,2],\"e\":{\"a\":0}},\"a\":{}}";
static constexpr char sk_dictTestInput_02_c[] =
"{\"a\":{},\"b\":{\"e\":{\"a\":0},\"c\":[0,1,2]}}";
static constexpr char sk_dictTestInput_02_d[] =
"{\"b\":{\"e\":{\"a\":0},\"c\":[0,1,2]},\"a\":{}}";
}

GTEST_TEST(TestJsonWriter, ListWriter)
{
	GenericObjectParser parser;

	{
		auto inputObj = parser.ParseTillEnd(sk_listTestInput_01);

		WriterConfig cfg;
		cfg.m_indent = "\t";

		std::string res = DumpStr(inputObj, cfg);

		EXPECT_EQ(res, sk_listTestInput_01);
	}

	{
		auto inputObj = parser.ParseTillEnd(sk_listTestInput_02);

		WriterConfig cfg;

		std::string res = DumpStr(inputObj, cfg);

		EXPECT_EQ(res, sk_listTestInput_02);
	}
}

GTEST_TEST(TestJsonWriter, DictWriter)
{
	{
		auto inputObj = LoadStr(sk_dictTestInput_01_a);

		WriterConfig cfg;
		cfg.m_indent = "\t";

		std::string res = DumpStr(inputObj, cfg);

		EXPECT_TRUE((res == sk_dictTestInput_01_a) ||
					(res == sk_dictTestInput_01_b) ||
					(res == sk_dictTestInput_01_c) ||
					(res == sk_dictTestInput_01_d));
	}

	{
		auto inputObj = LoadStr(sk_dictTestInput_01_a);

		std::string res = DumpStr(inputObj);

		EXPECT_TRUE((res == sk_dictTestInput_02_a) ||
					(res == sk_dictTestInput_02_b) ||
					(res == sk_dictTestInput_02_c) ||
					(res == sk_dictTestInput_02_d));
	}
}

GTEST_TEST(TestJsonWriter, ObjectWriter)
{
	// null key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::Null(), Internal::Obj::Null()}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"null\":null}");
	}

	// true key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::Bool(true), Internal::Obj::Bool(true)}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"true\":true}");
	}

	// false key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::Bool(false), Internal::Obj::Bool(false)}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"false\":false}");
	}

	// int key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::Int64(123456), Internal::Obj::Int64(123456)}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"123456\":123456}");
	}

	// float key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::Double(0.0), Internal::Obj::Double(0.0)}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"0.0\":0.0}");
	}

	// string key
	{
		Internal::Obj::Object testObj = Internal::Obj::Dict({
			{Internal::Obj::String("string"), Internal::Obj::String("string")}
		});

		std::string res = DumpStr(testObj);
		EXPECT_EQ(res, "{\"string\":\"string\"}");
	}
}
