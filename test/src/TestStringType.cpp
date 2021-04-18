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

GTEST_TEST(TestStringType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestString, ParseString)
{
	{
		constexpr char testInput[]  = " \t\"ABCDE\x31\x05\t\\b\\f\\n\\r\\t\\\"\\\\\\/\xF0\x9F\x98\x82\\ud83d\\uDE06\"  \r\n ";
		constexpr char testResult[] = "ABCDE1\x05\t\b\f\n\r\t\"\\/\xF0\x9F\x98\x82\xF0\x9F\x98\x86";
		constexpr char testOutput[] = "\"ABCDE1\\u0005\\t\\b\\f\\n\\r\\t\\\"\\\\\\/\\uD83D\\uDE02\\uD83D\\uDE06\"";

		auto str = String<>::Parse(std::begin(testInput), std::end(testInput) - 1);

		std::string testOutStr;
		str.ToString(std::back_inserter(testOutStr));

		EXPECT_EQ(str.GetUtf8String(), std::string(testResult));
		EXPECT_EQ(testOutStr, std::string(testOutput));

		auto str2 = String<>::Parse(testOutStr.begin(), testOutStr.end());

		EXPECT_EQ(str.GetUtf8String(), std::string(testResult));
	}
}

GTEST_TEST(TestString, ParseCorrect)
{
	{
		std::string testInput = " \t\"ABCDE\x31\x05\t\\b\\f\\n\\r\\t\\\"\\\\\\/\xF0\x9F\x98\x82\\ud83d\\uDE06\"  \r\n ";
		EXPECT_NO_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			String<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = String<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj.GetUtf8String(), "ABCDE1\x05\t\b\f\n\r\t\"\\/\xF0\x9F\x98\x82\xF0\x9F\x98\x86");
	}

	{
		std::string testInput = " \t\" !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\"  \r\n ";
		EXPECT_NO_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			String<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = String<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj.GetUtf8String(), " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}");
	}

	{
		std::string testInput = " \r\n \"abcdefghijklmnopqrstuvwxyz{|}\",   ";
		EXPECT_NO_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestStringType, ParseCorrect)
{
	{
		std::string testInput = " \t\"ABCDE\x31\x05\t\\b\\f\\n\\r\\t\\\"\\\\\\/\xF0\x9F\x98\x82\\ud83d\\uDE06\"  \r\n ";

		EXPECT_NO_THROW((
			Parser::StringTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::StringTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Json::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj->GetUtf8String(), "ABCDE1\x05\t\b\f\n\r\t\"\\/\xF0\x9F\x98\x82\xF0\x9F\x98\x86");
	}

	{
		std::string testInput = " \t\" !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\"  \r\n ";

		EXPECT_NO_THROW((
			Parser::StringTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::StringTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Json::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj->GetUtf8String(), " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}");
	}

	{
		std::string testInput = " \r\n \"abcdefghijklmnopqrstuvwxyz{|}\",   ";

		EXPECT_NO_THROW((
			Parser::StringTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestString, ParseError)
{
	// No Ending quote
	{
		std::string testInput = "  \"ABCDE   ";

		EXPECT_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);
	}
	// Invalid escape
	{
		std::string testInput = "  \"ABC\\DE\"   ";

		EXPECT_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);
	}
	// Invalid \uXXXX escape
	{
		std::string testInput = "  \"ABC\\uABCTDE\"   ";

		EXPECT_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);
	}
	// Invalid UTF-8
	{
		std::string testInput = "  \"ABC\x80 DE\"   ";

		EXPECT_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);
	}
	// No quotes
	{
		std::string testInput = "  ABCDEF   ";

		EXPECT_THROW((
			String<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);
	}
	// Invalid coding
	{
		char16_t testInput[] = { 'A', 'B', 'C', 'D', 0x8846, 'F' };

		EXPECT_THROW((
			String<>::ParsePartial(std::begin(testInput), std::end(testInput) - 1, std::begin(testInput))
		), ParseError);
	}
	// Extra Data
	{
		std::string testInput = " \r\n \"abcdefghijklmnopqrstuvwxyz{|}\",   ";

		EXPECT_THROW((
			String<>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
}

GTEST_TEST(TestString, ToString)
{
	{
		std::string testInput = " \t\"ABCDE\x31\x05\t\\b\\f\\n\\r\\t\\\"\\\\\\/\xF0\x9F\x98\x82\\ud83d\\uDE06\"  \r\n ";

		std::string testOut;

		String<>::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut));
		EXPECT_EQ(testOut, "\"ABCDE1\\u0005\\t\\b\\f\\n\\r\\t\\\"\\\\\\/\\uD83D\\uDE02\\uD83D\\uDE06\"");
		EXPECT_EQ(String<>::Parse(testInput.begin(), testInput.end()), String<>::Parse(testOut.begin(), testOut.end()));
	}

	{
		std::string testInput = " \t\" !\\\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\"  \r\n ";

		std::string testOut;

		String<>::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut), "\t");
		EXPECT_EQ(testOut, "\" !\\\"#$%&'()*+,-.\\/0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}\"\n");
		EXPECT_EQ(String<>::Parse(testInput.begin(), testInput.end()), String<>::Parse(testOut.begin(), testOut.end()));
	}
}
