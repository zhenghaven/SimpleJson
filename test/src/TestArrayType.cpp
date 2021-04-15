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

GTEST_TEST(TestArrayType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestVector, ParseCorrect)
{
	{
		std::string testInput = "  \t [ true, true,false, true,true,false  ]   ";
		EXPECT_NO_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		));
	}

	{
		std::string testInput = "  \t [ true, true,false, true,true,false,]   ";
		EXPECT_NO_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		));
	}

	{
		std::string testInput = "  \t [   ]   ";
		EXPECT_NO_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		));
	}

	{
		std::string testInput = "  \t []   ";
		EXPECT_NO_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		));
	}

	{
		std::string testInput = " \r\n [ true, true,false, true,true,false,],   ";
		EXPECT_NO_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestArrayType, ParseCorrect)
{
	{
		std::string testInput = "  \t [ true, null,false, true,null,false  ]   ";

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NO_THROW((obj->GetArray()));
		EXPECT_EQ(obj->GetArray().size(), 6);

		EXPECT_EQ(obj->GetArray()[0], BoolType(true));
		EXPECT_EQ(obj->GetArray()[1], NullType());
		EXPECT_EQ(obj->GetArray()[2], BoolType(false));
		EXPECT_EQ(obj->GetArray()[3], BoolType(true));
		EXPECT_EQ(obj->GetArray()[4], NullType());
		EXPECT_EQ(obj->GetArray()[5], BoolType(false));

		EXPECT_NE(obj->GetArray()[0], BoolType(false));
		EXPECT_NE(obj->GetArray()[0], NullType());
	}

	{
		std::string testInput = "  \t [ true, null,false, true,null,false,]   ";

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NO_THROW((obj->GetArray()));
		EXPECT_EQ(obj->GetArray().size(), 6);

		EXPECT_EQ(obj->GetArray()[0], BoolType(true));
		EXPECT_EQ(obj->GetArray()[1], NullType());
		EXPECT_EQ(obj->GetArray()[2], BoolType(false));
		EXPECT_EQ(obj->GetArray()[3], BoolType(true));
		EXPECT_EQ(obj->GetArray()[4], NullType());
		EXPECT_EQ(obj->GetArray()[5], BoolType(false));

		EXPECT_NE(obj->GetArray()[0], BoolType(false));
		EXPECT_NE(obj->GetArray()[0], NullType());
	}

	{
		std::string testInput = "  \t [   ]   ";

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NO_THROW((obj->GetArray()));
		EXPECT_EQ(obj->GetArray().size(), 0);
	}

	{
		std::string testInput = "  \t []   ";

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NO_THROW((obj->GetArray()));
		EXPECT_EQ(obj->GetArray().size(), 0);
	}

	{
		std::string testInput = " \r\n [ true, true,false, true,true,false,],   ";

		EXPECT_NO_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestVector, ParseError)
{
	{
		std::string testInput = "  < true, true,false, true,true,false,]   ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, True,false, true,true,false,]   ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, true,false, true,true false,]   ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, true,false, true,true,false,    ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, true,false, true,true,false    ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ , ]    ";

		EXPECT_THROW((
			Vector<Bool>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n [ true, true,false, true,true,false,],   ";

		EXPECT_THROW((
			Vector<Bool>::Parse(testInput.begin(), testInput.end())
		), ParseError);
		//auto obj = Vector<Bool>::Parse(testInput.begin(), testInput.end());
	}
}

GTEST_TEST(TestArrayType, ParseError)
{
	{
		std::string testInput = "  < true, null,false, true,null,false,]   ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, Null,false, true,null,false,]   ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, null,false, true,null false,]   ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, null,false, true,null,false,    ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [ true, null,false, true,null,false    ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}
	{
		std::string testInput = "  [  , ]    ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n [ true, null,false, true,null,false,],   ";

		EXPECT_THROW((
			Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())
		), ParseError);

		EXPECT_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		), ParseError);

		//auto obj = Parser::ArrayTypeParser().Parse(testInput);
	}
}

GTEST_TEST(TestArray, ToString)
{
	{
		std::string testInput = "[[[true,false],[false]],[[],[false,true]],[[]]]";

		std::string testOut;

		Vector<Vector<Vector<Bool> > >::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut));
		EXPECT_EQ(testOut, "[[[true,false],[false]],[[],[false,true]],[[]]]");
	}

	{
		std::string testInput = "[[[true,false],[false]],[[],[false,true]],[[]]]";

		std::string testOut;

		Vector<Vector<Vector<Bool> > >::Parse(testInput.begin(), testInput.end()).ToString(std::back_inserter(testOut), "\t");
		EXPECT_EQ(testOut, "[\n\t[\n\t\t[\n\t\t\ttrue,\n\t\t\tfalse\n\t\t],\n\t\t[\n\t\t\tfalse\n\t\t]\n\t],\n\t[\n\t\t[],\n\t\t[\n\t\t\tfalse,\n\t\t\ttrue\n\t\t]\n\t],\n\t[\n\t\t[]\n\t]\n]\n");
	}
}

GTEST_TEST(TestArrayType, ToString)
{
	{
		std::string testInput = "[[[true,false],[null]],[[],[null,true]],[[]]]";

		std::string testOut;

		testOut = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "[[[true,false],[null]],[[],[null,true]],[[]]]");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString();
		EXPECT_EQ(testOut, "[[[true,false],[null]],[[],[null,true]],[[]]]");
	}

	{
		std::string testInput = "[[[true,false],[null]],[[],[null,true]],[[]]]";

		std::string testOut;

		testOut = Parser::ArrayTypeParser<>().Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "[\n\t[\n\t\t[\n\t\t\ttrue,\n\t\t\tfalse\n\t\t],\n\t\t[\n\t\t\tnull\n\t\t]\n\t],\n\t[\n\t\t[],\n\t\t[\n\t\t\tnull,\n\t\t\ttrue\n\t\t]\n\t],\n\t[\n\t\t[]\n\t]\n]\n");

		testOut = Json::Parse(testInput.begin(), testInput.end())->ToString("\t");
		EXPECT_EQ(testOut, "[\n\t[\n\t\t[\n\t\t\ttrue,\n\t\t\tfalse\n\t\t],\n\t\t[\n\t\t\tnull\n\t\t]\n\t],\n\t[\n\t\t[],\n\t\t[\n\t\t\tnull,\n\t\t\ttrue\n\t\t]\n\t],\n\t[\n\t\t[]\n\t]\n]\n");
	}
}
