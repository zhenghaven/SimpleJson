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

GTEST_TEST(TestIntType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestInt, ParseCorrect)
{
	{
		std::string testInput = " \t 1234567890   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Integer<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj, 1234567890);
	}
	{
		std::string testInput = " \t -1234567890   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Integer<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj, -1234567890);
	}

	{
		std::string testInput = " \r\n 1234567890,   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t 0   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Integer<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj, 0);
	}

	{
		std::string testInput = " \t -0   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Integer<>::Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj, 0);
	}

	{
		std::string testInput = " \r\n 0,   ";
		EXPECT_NO_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestIntType, ParseCorrect)
{
	{
		std::string testInput = " \t 1234567890   ";

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj->GetInt(), 1234567890);
	}

	{
		std::string testInput = " \r\n 1234567890,   ";

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t -0   ";

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_EQ(obj->GetInt(), 0);
	}

	{
		std::string testInput = " \r\n 0,   ";

		EXPECT_NO_THROW((
			Parser::IntTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestInt, ParseError)
{
	{
		std::string testInput = "  1.0   ";

		EXPECT_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  1e10   ";

		EXPECT_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  1.1e10   ";

		EXPECT_THROW((
			Integer<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n 123,   ";

		EXPECT_THROW((
			Integer<>::Parse(testInput.begin(), testInput.end())
		), ParseError);

		//auto obj = Bool::Parse(testInput.begin(), testInput.end());
	}
}

GTEST_TEST(TestInt, ToString)
{
	{
		std::string testInput = "-123";

		std::string testOut;

		Integer<>::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut));
		EXPECT_EQ(testOut, "-123");
	}

	{
		std::string testInput = "123";

		std::string testOut;

		Integer<>::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut), "\t");
		EXPECT_EQ(testOut, "123\n");
	}
}

GTEST_TEST(TestIntType, ToString)
{
	{
		std::string testInput = "-0";

		std::string testOut;

		testOut = Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "0");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "0");
	}

	{
		std::string testInput = "987654321";

		std::string testOut;

		testOut = Parser::IntTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "987654321\n");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "987654321\n");
	}
}
