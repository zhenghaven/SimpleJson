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

GTEST_TEST(TestRealNumParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

static std::pair<std::string, std::string> AssembleTestInternalParser(
	const std::string& prefix,
	const std::string& iPart,
	const std::string& fPart,
	const std::string& suffix
)
{
	std::string testInput = prefix + iPart + fPart + suffix;

	using _IteratorType = Internal::Obj::FrIterator<char, true>;
	using _ISMType      = ForwardIteratorStateMachine<_IteratorType>;
	_ISMType ism(
		Internal::Obj::ToFrIt<true>(testInput.cbegin()),
		Internal::Obj::ToFrIt<true>(testInput.cend()));
	ism.SkipWhiteSpace();

	std::string iRes;
	std::string fRes;
	std::tie(iRes, fRes) = Internal::ParseNum<std::string>(ism);

	return std::make_pair(iRes, fRes);
}

static void TestInternalParserCorrect(
	const std::string& prefix,
	const std::string& iPart,
	const std::string& fPart,
	const std::string& suffix
)
{
	std::string iRes;
	std::string fRes;

	EXPECT_NO_THROW(
		std::tie(iRes, fRes) = AssembleTestInternalParser(
			prefix, iPart, fPart, suffix);
	);

	EXPECT_EQ(iPart, iRes);
	EXPECT_EQ(fPart, fRes);
}

static void TestInternalParserIncorrect(
	const std::string& prefix,
	const std::string& iPart,
	const std::string& fPart,
	const std::string& suffix
)
{
	EXPECT_THROW(
		std::tie(std::ignore, std::ignore) = AssembleTestInternalParser(
			prefix, iPart, fPart, suffix);,
		ParseError
	);
}

GTEST_TEST(TestRealNumParser, InternalParserCorrect)
{
	TestInternalParserCorrect("   \r\n  ",
		"0", "",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"-0", "",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"0", ".0",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"123456789", "e12",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"123456789", "e-12",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"123456789", ".123e-12",
		"   ;  \t   ");
	TestInternalParserCorrect("   \r\n  ",
		"123456789", "",
		"");

	// capital 'E'
	{
		std::string iRes;
		std::string fRes;
		EXPECT_NO_THROW(
			std::tie(iRes, fRes) = AssembleTestInternalParser(
				"   \r\n  ", "123", ".456E789", "   ;  \t   ");
		);
		EXPECT_EQ("123"      , iRes);
		EXPECT_EQ(".456e789", fRes);
	}
	// capital 'E'
	{
		std::string iRes;
		std::string fRes;
		EXPECT_NO_THROW(
			std::tie(iRes, fRes) = AssembleTestInternalParser(
				"   \r\n  ", "123", ".456E-789", "   ;  \t   ");
		);
		EXPECT_EQ("123"      , iRes);
		EXPECT_EQ(".456e-789", fRes);
	}
	// capital 'E' & positive
	{
		std::string iRes;
		std::string fRes;
		EXPECT_NO_THROW(
			std::tie(iRes, fRes) = AssembleTestInternalParser(
				"   \r\n  ", "123", ".456E+789", "   ;  \t   ");
		);
		EXPECT_EQ("123"      , iRes);
		EXPECT_EQ(".456e789", fRes);
	}
	// positive 'E'
	{
		std::string iRes;
		std::string fRes;
		EXPECT_NO_THROW(
			std::tie(iRes, fRes) = AssembleTestInternalParser(
				"   \r\n  ", "123", ".456e+789", "   ;  \t   ");
		);
		EXPECT_EQ("123"      , iRes);
		EXPECT_EQ(".456e789", fRes);
	}
}

GTEST_TEST(TestRealNumParser, InternalParserIncorrect)
{
	TestInternalParserIncorrect("   \r\n  ",
		"", "",
		"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"-", ".123",
		"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"", ".123",
		"   ;  \t   ");
	// TestInternalParserIncorrect("   \r\n  ",
	// 	"123.456", ".789",
	// 	"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"+0", "",
		"   ;  \t   ");
	// TestInternalParserIncorrect("   \r\n  ",
	// 	"0123", "",
	// 	"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"", ".123",
		"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"", "e10",
		"   ;  \t   ");
	TestInternalParserIncorrect("   \r\n  ",
		"", "e-10",
		"   ;  \t   ");

	using _IteratorType = Internal::Obj::FrIterator<char, true>;
	using _ISMType      = ForwardIteratorStateMachine<_IteratorType>;
	std::string ignoreStr;

	{
		std::string testInput = " ";

		_ISMType ism(
			Internal::Obj::ToFrIt<true>(testInput.cbegin()),
			Internal::Obj::ToFrIt<true>(testInput.cend()));
		ism.SkipWhiteSpace();

		EXPECT_THROW(
			Internal::ParseNumDigits(ism, std::back_inserter(ignoreStr));,
			ParseError
		);
	}
	{
		std::string testInput = "X";

		_ISMType ism(
			Internal::Obj::ToFrIt<true>(testInput.cbegin()),
			Internal::Obj::ToFrIt<true>(testInput.cend()));

		EXPECT_THROW(
			Internal::ParseNumDigits(ism, std::back_inserter(ignoreStr));,
			ParseError
		);
	}
}

// GenericNum

GTEST_TEST(TestNullParser, GenericNumParseCorrect)
{
	using namespace Internal::Obj;

	GenericNumberParser parser;
	{
		std::string testInput = " \t 123456789   ";
		Object res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Int64));
		EXPECT_EQ(res, Object(Int64(123456789LL)));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Int64));
		EXPECT_EQ(res, Object(Int64(123456789LL)));
	}

	{
		std::string testInput = " \r\n 123456789;   ";
		Object res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Int64));
		EXPECT_EQ(res, Object(Int64(123456789LL)));
	}

	{
		std::string testInput = " \t 12345.6789   ";
		Object res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Double));
		EXPECT_EQ(res, Object(Double(12345.6789)));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Double));
		EXPECT_EQ(res, Object(Double(12345.6789)));
	}

	{
		std::string testInput = " \r\n 12345.6789;   ";
		Object res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_NO_THROW(
			EXPECT_TRUE(
				res.AsRealNum().GetNumType() ==
				RealNumType::Double));
		EXPECT_EQ(res, Object(Double(12345.6789)));
	}
}

GTEST_TEST(TestNullParser, GenericNumParseError)
{
	GenericNumberParser parser;
	{
		std::string testInput = " \t -.123   ";
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
		std::string testInput = " \t -.123   ";
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
		std::string testInput = " \r\n 123456789;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}

// IntegerParse

GTEST_TEST(TestNullParser, IntegerParseCorrect)
{
	IntegerParser parser;
	{
		std::string testInput = " \t 123456789   ";
		Internal::Obj::Int64 res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Int64(123456789LL));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Int64(123456789LL));
	}

	{
		std::string testInput = " \r\n 123456789;   ";
		Internal::Obj::Int64 res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Int64(123456789LL));
	}
}

GTEST_TEST(TestNullParser, IntegerParseError)
{
	IntegerParser parser;
	{
		std::string testInput = " \t 12345.6789   ";
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
		std::string testInput = " \t -.6789   ";
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
		std::string testInput = " \r\n 123456789;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}

// RealNumParse

GTEST_TEST(TestNullParser, RealNumParseCorrect)
{
	RealNumParser parser;
	{
		std::string testInput = " \t 12345.6789   ";
		Internal::Obj::Double res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Double(12345.6789));
		EXPECT_NO_THROW(
			res = parser.ParseTillEnd(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Double(12345.6789));
	}

	{
		std::string testInput = " \r\n 12345.6789;   ";
		Internal::Obj::Double res;
		EXPECT_NO_THROW(
			res = parser.Parse(testInput);
		);
		EXPECT_EQ(res, Internal::Obj::Double(12345.6789));
	}
}

GTEST_TEST(TestNullParser, RealNumParseError)
{
	RealNumParser parser;
	{
		std::string testInput = " \t 123456789   ";
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
		std::string testInput = " \t -.6789   ";
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
		std::string testInput = " \r\n 12345.6789;   ";
		EXPECT_THROW(
			parser.ParseTillEnd(testInput);,
			ParseError
		);
	}
}
