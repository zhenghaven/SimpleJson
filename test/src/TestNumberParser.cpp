#include <gtest/gtest.h>

#include <SimpleJson/Internal/NumberParser.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestNumberParser, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

static void TestParseNumberCorrect(
	char sign,
	const std::string& expIntStr,
	const std::string& expFracStr,
	char expPrefix,
	char expSign,
	const std::string& expExpStr
)
{
	bool expIsPos = true;
	bool expIsExpPos = false;

	std::string testInput = "";

	if (sign != '\0')
	{
		testInput += sign;
		expIsPos = false;
	}

	testInput += expIntStr;

	if(expFracStr.size() > 0)
	{
		testInput += '.';
		testInput += expFracStr;
	}

	if(expExpStr.size() > 0)
	{
		expIsExpPos = true;

		testInput += expPrefix;
		if (expSign != '\0')
		{
			testInput += expSign;
			expIsExpPos = expSign == '-' ? false : true;
		}
		testInput += expExpStr;
	}

	bool isPos = false;
	std::string intStr;
	std::string fracStr;
	bool isExpPos = false;
	std::string expStr;

	auto begin = testInput.begin();
	EXPECT_NO_THROW(
		Parser::Internal::ParseNumber(begin, testInput.end(), testInput.begin(),
			isPos, std::back_inserter(intStr),
			std::back_inserter(fracStr),
			isExpPos, std::back_inserter(expStr));
	);

	EXPECT_EQ(isPos, expIsPos);
	EXPECT_EQ(intStr, expIntStr);
	EXPECT_EQ(fracStr, expFracStr);
	EXPECT_EQ(isExpPos, expIsExpPos);
	EXPECT_EQ(expStr, expExpStr);
}

GTEST_TEST(TestNumberParser, ParseCorrect)
{
	TestParseNumberCorrect(
		'\0',
		"12345678900",
		"",
		'E',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'-',
		"12345678900",
		"",
		'E',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'\0',
		"0",
		"",
		'E',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'-',
		"0",
		"",
		'E',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'\0',
		"0",
		"0",
		'E',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'\0',
		"0",
		"0",
		'e',
		'\0',
		"0"
	);
	TestParseNumberCorrect(
		'-',
		"0",
		"0",
		'e',
		'-',
		"0"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"001234500",
		'e',
		'\0',
		""
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"001234500",
		'E',
		'+',
		"123"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"001234500",
		'e',
		'-',
		"123"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"001234500",
		'e',
		'\0',
		"123"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"",
		'E',
		'+',
		"123"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"",
		'e',
		'-',
		"123"
	);
	TestParseNumberCorrect(
		'\0',
		"1234500",
		"",
		'E',
		'\0',
		"123"
	);
}

static void TestParseNumberFailed(
	const std::string& testInput,

	bool expIsPos,
	const std::string& expIntStr,
	const std::string& expFracStr,
	bool expIsExpPos,
	const std::string& expExpStr
)
{
	bool isPos = false;
	std::string intStr;
	std::string fracStr;
	bool isExpPos = false;
	std::string expStr;

	auto begin = testInput.begin();
	EXPECT_THROW(
		(
			Parser::Internal::ParseNumber(begin, testInput.end(), testInput.begin(),
				isPos, std::back_inserter(intStr),
				std::back_inserter(fracStr),
				isExpPos, std::back_inserter(expStr))
		)
		,
		ParseError
	);

	EXPECT_EQ(isPos, expIsPos);
	EXPECT_EQ(intStr, expIntStr);
	EXPECT_EQ(fracStr, expFracStr);
	EXPECT_EQ(isExpPos, expIsExpPos);
	EXPECT_EQ(expStr, expExpStr);
}

GTEST_TEST(TestNumberParser, ParseFailed)
{
	TestParseNumberFailed("-",
		false, "", "", false, "");

	TestParseNumberFailed("- 123",
		false, "", "", false, "");

	TestParseNumberFailed("+",
		true, "", "", false, "");

	TestParseNumberFailed("+0",
		true, "", "", false, "");

	TestParseNumberFailed("0.",
		true, "0", "", false, "");

	TestParseNumberFailed("123.",
		true, "123", "", false, "");

	TestParseNumberFailed("123. 123",
		true, "123", "", false, "");

	TestParseNumberFailed("0.-1",
		true, "0", "", false, "");

	TestParseNumberFailed("0.0e",
		true, "0", "0", true, "");

	TestParseNumberFailed("0.123e",
		true, "0", "123", true, "");

	TestParseNumberFailed("0.123e 123",
		true, "0", "123", true, "");

	TestParseNumberFailed("0.123e -123",
		true, "0", "123", true, "");

	TestParseNumberFailed("0.123e +123",
		true, "0", "123", true, "");

	TestParseNumberFailed("0.0e-",
		true, "0", "0", false, "");

	TestParseNumberFailed("0.0e- 123",
		true, "0", "0", false, "");

	TestParseNumberFailed("0.0e+",
		true, "0", "0", true, "");

	TestParseNumberFailed("0.0e+ 123",
		true, "0", "0", true, "");

	TestParseNumberFailed("0.0e--1",
		true, "0", "0", false, "");

	TestParseNumberFailed("0.0e++1",
		true, "0", "0", true, "");
}

static void TestParseNumberExtra(
	const std::string& testInput,

	bool expIsPos,
	const std::string& expIntStr,
	const std::string& expFracStr,
	bool expIsExpPos,
	const std::string& expExpStr,

	const std::string& expExtra
)
{
	bool isPos = false;
	std::string intStr;
	std::string fracStr;
	bool isExpPos = false;
	std::string expStr;

	auto begin = testInput.begin();
	EXPECT_NO_THROW(
		Parser::Internal::ParseNumber(begin, testInput.end(), testInput.begin(),
			isPos, std::back_inserter(intStr),
			std::back_inserter(fracStr),
			isExpPos, std::back_inserter(expStr))
	);

	EXPECT_EQ(isPos, expIsPos);
	EXPECT_EQ(intStr, expIntStr);
	EXPECT_EQ(fracStr, expFracStr);
	EXPECT_EQ(isExpPos, expIsExpPos);
	EXPECT_EQ(expStr, expExpStr);

	EXPECT_EQ(std::string(begin, testInput.end()), expExtra);
}

GTEST_TEST(TestNumberParser, ParseExtra)
{
	TestParseNumberExtra("-0123",
		false, "0", "", false, "", "123");

	TestParseNumberExtra("0123",
		true, "0", "", false, "", "123");

	TestParseNumberExtra("123.123.123",
		true, "123", "123", false, "", ".123");

	TestParseNumberExtra("123.123e123.123",
		true, "123", "123", true, "123", ".123");

	TestParseNumberExtra("123.123e123-123",
		true, "123", "123", true, "123", "-123");

	TestParseNumberExtra("123 45",
		true, "123", "", false, "", " 45");

	TestParseNumberExtra("123.123 45",
		true, "123", "123", false, "", " 45");

	TestParseNumberExtra("123 .123",
		true, "123", "", false, "", " .123");

	TestParseNumberExtra("123.123 e123",
		true, "123", "123", false, "", " e123");

	TestParseNumberExtra("123.123e123 45",
		true, "123", "123", true, "123", " 45");
}
