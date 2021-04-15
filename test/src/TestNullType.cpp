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

GTEST_TEST(TestNullType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestNull, ParseCorrect)
{
	{
		std::string testInput = " \t null   ";
		EXPECT_NO_THROW((
			Null::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Null::Parse(testInput.begin(), testInput.end())
		));
	}

	{
		std::string testInput = " \r\n null;   ";
		EXPECT_NO_THROW((
			Null::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestNullType, ParseCorrect)
{
	{
		std::string testInput = " \t null   ";

		EXPECT_NO_THROW((
			Parser::NullTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto nullObj = Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_TRUE(nullObj->IsNull());
	}

	{
		std::string testInput = " \r\n null;   ";

		EXPECT_NO_THROW((
			Parser::NullTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestNull, ParseError)
{
	{
		std::string testInput = "  Null   ";

		EXPECT_THROW((
			Null::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Null::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \nnull.   ";

		EXPECT_THROW((
			Null::Parse(testInput.begin(), testInput.end())
		), ParseError);
		//auto nullObj = Null::Parse(testInput.begin(), testInput.end());
	}
}

GTEST_TEST(TestNullType, ParseError)
{
	{
		std::string testInput = "  Null   ";

		EXPECT_THROW((
			Parser::NullTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \nnull.   ";

		EXPECT_THROW((
			Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);

		//auto nullObj = Parser::NullTypeParser().Parse(testInput);
	}
}

GTEST_TEST(TestNull, ToString)
{
	{
		std::string testInput = "null";

		std::string testOut;

		Null::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut));
		EXPECT_EQ(testOut, "null");
	}

	{
		std::string testInput = "null";

		std::string testOut;

		Null::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut), "\t");
		EXPECT_EQ(testOut, "null\n");
	}
}

GTEST_TEST(TestNullType, ToString)
{
	{
		std::string testInput = "null";

		std::string testOut;

		testOut = Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "null");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "null");
	}

	{
		std::string testInput = "null";

		std::string testOut;

		testOut = Parser::NullTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "null\n");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "null\n");
	}
}
