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

GTEST_TEST(TestBoolType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestBool, ParseCorrect)
{
	{
		std::string testInput = " \t true   ";
		EXPECT_NO_THROW((
			Bool::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Bool::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Bool::Parse(testInput.begin(), testInput.end());
		EXPECT_TRUE(obj);
	}

	{
		std::string testInput = " \r\n true,   ";
		EXPECT_NO_THROW((
			Bool::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t false   ";
		EXPECT_NO_THROW((
			Bool::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Bool::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Bool::Parse(testInput.begin(), testInput.end());
		EXPECT_FALSE(obj);
	}

	{
		std::string testInput = " \r\n false,   ";
		EXPECT_NO_THROW((
			Bool::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestBoolType, ParseCorrect)
{
	{
		std::string testInput = " \t true   ";

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_TRUE(obj->GetBool());
	}

	{
		std::string testInput = " \r\n true,   ";

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t false   ";

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_FALSE(obj->GetBool());
	}

	{
		std::string testInput = " \r\n false,   ";

		EXPECT_NO_THROW((
			Parser::BoolTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestBool, ParseError)
{
	{
		std::string testInput = "  True   ";

		EXPECT_THROW((
			Bool::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Bool::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n true,   ";

		EXPECT_THROW((
			Bool::Parse(testInput.begin(), testInput.end())
		), ParseError);

		//auto obj = Bool::Parse(testInput.begin(), testInput.end());
	}
}

GTEST_TEST(TestBoolType, ParseError)
{
	{
		std::string testInput = "  False   ";

		EXPECT_THROW((
			Parser::BoolTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n false,   ";

		EXPECT_THROW((
			Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
		//auto obj = Parser::BoolTypeParser().Parse(testInput);
	}
}

GTEST_TEST(TestBool, ToString)
{
	{
		std::string testInput = "true";

		std::string testOut;

		Bool::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut));
		EXPECT_EQ(testOut, "true");
	}

	{
		std::string testInput = "false";

		std::string testOut;

		Bool::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut), "\t");
		EXPECT_EQ(testOut, "false\n");
	}
}

GTEST_TEST(TestBoolType, ToString)
{
	{
		std::string testInput = "true";

		std::string testOut;

		testOut = Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "true");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "true");
	}

	{
		std::string testInput = "false";

		std::string testOut;

		testOut = Parser::BoolTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "false\n");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "false\n");
	}
}
