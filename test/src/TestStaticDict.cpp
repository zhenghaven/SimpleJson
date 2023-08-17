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

GTEST_TEST(TestStaticDictParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

namespace
{

// LCOV_EXCL_START
using TestStaticDict1 = SimpleObjects::StaticDict<
	std::tuple<
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key1_1")>,
			SimpleObjects::List> > >;

using TestStaticDict2 = SimpleObjects::StaticDict<
	std::tuple<
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_1")>,
			SimpleObjects::String>,
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_2")>,
			SimpleObjects::Int64>,
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_3")>,
			TestStaticDict1> > >;

template<bool _AllowMissingItem, bool _AllowExtraItem>
using TestStaticDict1ParserT = StaticDictParserT<
	std::tuple<
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key1_1")>,
			ListParserT<IntegerParser> > >,
	_AllowMissingItem,
	_AllowExtraItem>;

template<bool _AllowMissingItem, bool _AllowExtraItem>
using TestStaticDict2ParserT = StaticDictParserT<
	std::tuple<
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_1")>,
			StringParser>,
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_2")>,
			IntegerParser>,
		std::pair<SimpleObjects::StrKey<SIMOBJ_KSTR("Key2_3")>,
			TestStaticDict1ParserT<_AllowMissingItem, _AllowExtraItem> > >,
	_AllowMissingItem,
	_AllowExtraItem>;

// LCOV_EXCL_STOP

// Exact match
static constexpr char sk_testInput_01[] =
"{\n\
	\"Key2_1\" : \"val2_1\",\n\
	\"Key2_2\" : 22,\n\
	\"Key2_3\" : {\n\
		\"Key1_1\" : [\n\
			0,\n\
			1,\n\
			2\n\
		]\n\
	}\n\
}";
static constexpr char sk_testInput_01s[] =
"{\"Key2_1\":\"val2_1\",\"Key2_2\":22,\"Key2_3\":{\"Key1_1\":[0,1,2]}}";
// Duplicated item
static constexpr char sk_testInput_02[] =
"{\n\
	\"Key2_1\": \"val2_1\",\n\
	\"Key2_2\": 22,\n\
	\"Key2_3\": {\"Key1_1\":[0,1,2]},\n\
	\"Key2_2\": 999\n\
}";
// Missing item
static constexpr char sk_testInput_03[] =
"{\n\
	\"Key2_2\": 22,\n\
	\"Key2_3\": {\"Key1_1\":[0,1,2]}\n\
}";
// Extra item
static constexpr char sk_testInput_04[] =
"{\n\
	\"Key2_1\": \"val2_1\",\n\
	\"Key2_2\": 22,\n\
	\"Key2_3\": {\"Key1_1\":[0,1,2]},\n\
	\"Key2_4\": [0,1,2]\n\
}";
// All items are missing
static constexpr char sk_testInput_05[] =
"{\n\
}";
// Invalid value type
static constexpr char sk_testInput_06[] =
"{\n\
	\"Key2_1\": \"val2_1\",\n\
	\"Key2_2\": 22,\n\
	\"Key2_3\": [0,1,2]\n\
}";

} // namespace

GTEST_TEST(TestStaticDictParser, Construction)
{
	TestStaticDict1ParserT<false, false> dict1Parser;
	TestStaticDict2ParserT<false, false> dict2Parser;
}

GTEST_TEST(TestStaticDictParser, ParseCorrect)
{
	using namespace Internal::Obj;

	// Test on inner parser
	{
		ListParserT<IntegerParser> parser;
		auto testProg01 = [&]()
		{
			auto obj01 = parser.Parse("[0, 1, 2]");
			auto obj02 = parser.ParseTillEnd("[0, 1, 2]");
			auto expList = ListT<Int64>({ Int64(0), Int64(1), Int64(2) });
			EXPECT_EQ(obj01, obj02);
			EXPECT_EQ(obj02, expList);
		};
		EXPECT_NO_THROW(testProg01(););
	}

	// Exact match
	{
		TestStaticDict2ParserT<false, false> dict2Parser;
		auto testProg01 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_01);
			const auto& val2_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_1")> >();
			const auto& val2_2 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_2")> >();
			const auto& val2_3 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_3")> >();
			const auto& val1_1 = val2_3.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ Int64(0), Int64(1), Int64(2) });
			EXPECT_EQ(val2_1, String("val2_1"));
			EXPECT_EQ(val2_2, Int64(22));
			EXPECT_EQ(val1_1, expList);

			auto dict02 = dict2Parser.Parse(sk_testInput_01);
		};
		EXPECT_NO_THROW(testProg01(););
	}

	// Duplicated item
	{
		TestStaticDict2ParserT<false, false> dict2Parser;
		auto testProg02 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_02);
			const auto& val2_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_1")> >();
			const auto& val2_2 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_2")> >();
			const auto& val2_3 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_3")> >();
			const auto& val1_1 = val2_3.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ Int64(0), Int64(1), Int64(2) });
			EXPECT_EQ(val2_1, String("val2_1"));
			EXPECT_EQ(val2_2, Int64(999));
			EXPECT_EQ(val1_1, expList);
		};
		EXPECT_NO_THROW(testProg02(););
	}

	// Missing item
	{
		TestStaticDict2ParserT<true, false> dict2Parser;
		auto testProg03 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_03);
			const auto& val2_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_1")> >();
			const auto& val2_2 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_2")> >();
			const auto& val2_3 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_3")> >();
			const auto& val1_1 = val2_3.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ Int64(0), Int64(1), Int64(2) });
			EXPECT_EQ(val2_1, String(""));
			EXPECT_EQ(val2_2, Int64(22));
			EXPECT_EQ(val1_1, expList);
		};
		EXPECT_NO_THROW(testProg03(););
	}

	// Extra item
	{
		TestStaticDict2ParserT<false, true> dict2Parser;
		auto testProg04 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_04);
			const auto& val2_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_1")> >();
			const auto& val2_2 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_2")> >();
			const auto& val2_3 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_3")> >();
			const auto& val1_1 = val2_3.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ Int64(0), Int64(1), Int64(2) });
			EXPECT_EQ(val2_1, String("val2_1"));
			EXPECT_EQ(val2_2, Int64(22));
			EXPECT_EQ(val1_1, expList);
		};
		EXPECT_NO_THROW(testProg04(););
	}

	// All items are missing
	{
		TestStaticDict2ParserT<true, false> dict2Parser;
		auto testProg05 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_05);
			const auto& val2_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_1")> >();
			const auto& val2_2 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_2")> >();
			const auto& val2_3 = dict01.get<StrKey<SIMOBJ_KSTR("Key2_3")> >();
			const auto& val1_1 = val2_3.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ });
			EXPECT_EQ(val2_1, String(""));
			EXPECT_EQ(val2_2, Int64(0));
			EXPECT_EQ(val1_1, expList);
		};
		EXPECT_NO_THROW(testProg05(););

		TestStaticDict1ParserT<true, false> dict1Parser;
		auto testProg05_1 = [&]()
		{
			auto dict01 = dict1Parser.ParseTillEnd(sk_testInput_05);
			const auto& val1_1 = dict01.get<StrKey<SIMOBJ_KSTR("Key1_1")> >();
			auto expList = ListT<Int64>({ });
			EXPECT_EQ(val1_1, expList);

			auto dict02 = dict1Parser.Parse(sk_testInput_05);
		};
		EXPECT_NO_THROW(testProg05_1(););
	}
}

GTEST_TEST(TestStaticDictParser, ParseError)
{
	using namespace Internal::Obj;

	// not allow missing item
	{
		TestStaticDict2ParserT<false, true> dict2Parser;
		auto testProg01 = [&]()
		{
			dict2Parser.ParseTillEnd(sk_testInput_03);
		};
		EXPECT_THROW(testProg01();, ParseError);
	}

	// not allow extra item
	{
		TestStaticDict2ParserT<true, false> dict2Parser;
		auto testProg02 = [&]()
		{
			dict2Parser.ParseTillEnd(sk_testInput_04);
		};
		EXPECT_THROW(testProg02();, ParseError);
	}

	{
		TestStaticDict2ParserT<true, false> dict2Parser;
		auto testProg06 = [&]()
		{
			dict2Parser.ParseTillEnd(sk_testInput_06);
		};
		EXPECT_THROW(testProg06();, ParseError);
	}
}

GTEST_TEST(TestStaticDictParser, WriteStaticDict)
{
	{
		TestStaticDict2ParserT<false, false> dict2Parser;
		auto testProg01 = [&]()
		{
			auto dict01 = dict2Parser.ParseTillEnd(sk_testInput_01);

			WriterConfig cfg;
			std::string res;
			JsonWriterObject::Write(
				std::back_inserter(res),
				dict01,
				cfg,
				WriterStates()
			);

			EXPECT_EQ(res, sk_testInput_01s);

			res = "";
			cfg.m_indent = "\t";
			JsonWriterObject::Write(
				std::back_inserter(res),
				dict01,
				cfg,
				WriterStates()
			);

			EXPECT_EQ(res, sk_testInput_01);
		};
		EXPECT_NO_THROW(testProg01(););
	}
}
