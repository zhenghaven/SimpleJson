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
		true, "123", "", false, "", "45");

	TestParseNumberExtra("123.123 45",
		true, "123", "123", false, "", "45");

	TestParseNumberExtra("123 .123",
		true, "123", "", false, "", ".123");

	TestParseNumberExtra("123.123 e123",
		true, "123", "123", false, "", "e123");

	TestParseNumberExtra("123.123e123 45",
		true, "123", "123", true, "123", "45");
}

GTEST_TEST(TestNumberParser, ConversionCorrect)
{
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int8_t>("0", "", ""), 0);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int8_t>("-0", "", ""), 0);
	constexpr int32_t expResultN8 = -128;
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int8_t>("-128", "", ""), expResultN8);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int8_t>("127", "", ""), 127);
	constexpr int32_t expResultN16 = -32768;
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int16_t>("-32768", "", ""), expResultN16);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int16_t>("32767", "", ""), 32767);
	constexpr int32_t expResultN32 = -2147483647 - 1;
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int32_t>("-2147483648", "", ""), expResultN32);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int32_t>("2147483647", "", ""), 2147483647);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int64_t>("-9223372036854775808", "", ""), std::numeric_limits<int64_t>::min());
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<int64_t>("9223372036854775807", "", ""), std::numeric_limits<int64_t>::max());

	EXPECT_EQ(Parser::Internal::ComponentsToNumber<uint8_t>("255", "", ""), 255);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<uint16_t>("65535", "", ""), 65535);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<uint32_t>("4294967295", "", ""), 4294967295);
	EXPECT_EQ(Parser::Internal::ComponentsToNumber<uint64_t>("18446744073709551615", "", ""), std::numeric_limits<uint64_t>::max());

	EXPECT_NEAR(
		12345,
		Parser::Internal::ComponentsToNumber<float>("12345", "", "0"),
		0.01
	);
	EXPECT_NEAR(
		12345.6789,
		Parser::Internal::ComponentsToNumber<float>("12345", "6789", ""),
		0.01
	);
	EXPECT_NEAR(
		-12345.6789e33,
		Parser::Internal::ComponentsToNumber<float>("-12345", "6789", "33"),
		1e30
	);
	EXPECT_NEAR(
		12345.6789e-42,
		Parser::Internal::ComponentsToNumber<float>("12345", "6789", "-42"),
		0.01
	);

	EXPECT_NEAR(
		-12345.6789e303,
		Parser::Internal::ComponentsToNumber<double>("-12345", "6789", "+303"),
		1e300
	);
	EXPECT_NEAR(
		12345.6789e-310,
		Parser::Internal::ComponentsToNumber<double>("12345", "6789", "-310"),
		0.01
	);

	EXPECT_NEAR(
		-12345.6789e303,
		Parser::Internal::ComponentsToNumber<long double>("-12345", "6789", "+303"),
		1e300
	);
	EXPECT_NEAR(
		12345.6789e-310,
		Parser::Internal::ComponentsToNumber<long double>("12345", "6789", "-310"),
		0.01
	);
}

GTEST_TEST(TestNumberParser, ConversionOutOfRange)
{
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int8_t>("-129", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int8_t>("128", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int16_t>("-32769", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int16_t>("32768", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int32_t>("-2147483649", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int32_t>("2147483648", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int64_t>("-9223372036854775809", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<int64_t>("9223372036854775808", "", ""), RangeErrorException);

	EXPECT_THROW(Parser::Internal::ComponentsToNumber<uint8_t>("256", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<uint16_t>("65536", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<uint32_t>("4294967296", "", ""), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<uint32_t>("18446744073709551616", "", ""), RangeErrorException);

	EXPECT_THROW(Parser::Internal::ComponentsToNumber<float>("1", "2345", "1000"), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<float>("1", "2345", "-1000"), RangeErrorException);

	EXPECT_THROW(Parser::Internal::ComponentsToNumber<double>("1", "2345", "100000"), RangeErrorException);
	EXPECT_THROW(Parser::Internal::ComponentsToNumber<double>("1", "2345", "-100000"), RangeErrorException);
}
