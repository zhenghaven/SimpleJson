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

GTEST_TEST(TestRealType, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestReal, ParseCorrect)
{
	{
		std::string testInput = " \t 123.456e2   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Real<>::Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj.GetVal(), 123.456e2, 0.1e2);
	}
	{
		std::string testInput = " \t -123.456e-2   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Real<>::Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj.GetVal(), -123.456e-2, 0.1e-2);
	}

	{
		std::string testInput = " \r\n 123.456e2,   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t 0.0   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Real<>::Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj.GetVal(), 0, 0.1);
	}

	{
		std::string testInput = " \t -0.0   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Real<>::Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj.GetVal(), 0, 0.1);
	}

	{
		std::string testInput = " \r\n 0.0,   ";
		EXPECT_NO_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
}

GTEST_TEST(TestRealType, ParseCorrect)
{
	{
		std::string testInput = " \t 123.456e2   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj->GetReal(), 123.456e2, 0.1e2);
	}

	{
		std::string testInput = " \r\n 123.456e2,   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}
	{
		std::string testInput = " \t -123.456e-2   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj->GetReal(), -123.456e-2, 0.1e-2);
	}

	{
		std::string testInput = " \r\n -123.456e-2,   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t 0.0   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj->GetReal(), 0.0, 0.1);
	}

	{
		std::string testInput = " \r\n 0.0,   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \t -0.0   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NO_THROW((
			Json::Parse(testInput.begin(), testInput.end())
		));

		auto obj = Parser::RealTypeParser<>().Parse(testInput.begin(), testInput.end());
		EXPECT_NEAR(obj->GetReal(), 0.0, 0.1);
	}

	{
		std::string testInput = " \r\n -0.0,   ";

		EXPECT_NO_THROW((
			Parser::RealTypeParser<>().ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		));
	}

	{
		std::string testInput = " \r\n 0.0  ";
		std::unique_ptr<Json> obj;
		EXPECT_NO_THROW((
			obj = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj->GetDataType(), DataTypes::Real);
	}
	{
		std::string testInput = " \r\n 1e1  ";
		std::unique_ptr<Json> obj;
		EXPECT_NO_THROW((
			obj = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj->GetDataType(), DataTypes::Real);
	}
	{
		std::string testInput = " \r\n 1.1e1  ";
		std::unique_ptr<Json> obj;
		EXPECT_NO_THROW((
			obj = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj->GetDataType(), DataTypes::Real);
	}
	{
		std::string testInput = " \r\n 0  ";
		std::unique_ptr<Json> obj;
		EXPECT_NO_THROW((
			obj = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj->GetDataType(), DataTypes::Integer);
	}
}

GTEST_TEST(TestReal, ParseError)
{
	{
		std::string testInput = "  1   ";

		EXPECT_THROW((
			Real<>::ParsePartial(testInput.begin(), testInput.end(), testInput.begin())
		), ParseError);

		EXPECT_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		), ParseError);
	}

	{
		std::string testInput = "  \n 1.0,   ";

		EXPECT_THROW((
			Real<>::Parse(testInput.begin(), testInput.end())
		), ParseError);

	}
}

GTEST_TEST(TestReal, ToString)
{
	{
		std::string testInput = "0.12093823890283019387912081\t\t";
		Real<> obj1;
		Real<> obj2;
		EXPECT_NO_THROW((
			obj1 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		testInput.clear();
		obj1.ToString(std::back_inserter(testInput));

		EXPECT_NO_THROW((
			obj2 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NEAR(obj1.GetVal(), obj2.GetVal(), 0.001);
	}
	{
		std::string testInput = "1.2093e100\t\t";
		Real<> obj1;
		Real<> obj2;
		EXPECT_NO_THROW((
			obj1 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		testInput.clear();
		obj1.ToString(std::back_inserter(testInput));

		EXPECT_NO_THROW((
			obj2 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NEAR(obj1.GetVal(), obj2.GetVal(), 0.1e100);
	}
	{
		std::string testInput = "-1.2093e-100\t\t";
		Real<> obj1;
		Real<> obj2;
		EXPECT_NO_THROW((
			obj1 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		testInput.clear();
		obj1.ToString(std::back_inserter(testInput));

		EXPECT_NO_THROW((
			obj2 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NEAR(obj1.GetVal(), obj2.GetVal(), 0.1e-100);
	}
	{
		Real<> obj1 = 1;
		Real<> obj2;

		std::string testInput;
		obj1.ToString(std::back_inserter(testInput));
		//std::cout<< testInput << std::endl;

		EXPECT_NO_THROW((
			obj2 = Real<>::Parse(testInput.begin(), testInput.end())
		));

		EXPECT_NEAR(obj1.GetVal(), obj2.GetVal(), 0.1);
	}
}

GTEST_TEST(TestRealType, ToString)
{
	{
		std::string testInput = "0.12093823890283019387912081\t\t";
		std::unique_ptr<Json> obj1;
		std::unique_ptr<Json> obj2;
		EXPECT_NO_THROW((
			obj1 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj1->GetDataType(), DataTypes::Real);

		testInput.clear();
		obj1->ToString(std::back_inserter(testInput));
		//std::cout<< testInput << std::endl;
		EXPECT_NO_THROW((
			obj2 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj2->GetDataType(), DataTypes::Real);
		EXPECT_NEAR(obj1->GetReal(), obj2->GetReal(), 0.001);
	}
	{
		std::string testInput = "1.2093e100\t\t";
		std::unique_ptr<Json> obj1;
		std::unique_ptr<Json> obj2;
		EXPECT_NO_THROW((
			obj1 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj1->GetDataType(), DataTypes::Real);

		testInput.clear();
		obj1->ToString(std::back_inserter(testInput));
		//std::cout<< testInput << std::endl;
		EXPECT_NO_THROW((
			obj2 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj2->GetDataType(), DataTypes::Real);
		EXPECT_NEAR(obj1->GetReal(), obj2->GetReal(), 0.1e100);
	}
	{
		std::string testInput = "-1.2093e-100\t\t";
		std::unique_ptr<Json> obj1;
		std::unique_ptr<Json> obj2;
		EXPECT_NO_THROW((
			obj1 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj1->GetDataType(), DataTypes::Real);

		testInput.clear();
		obj1->ToString(std::back_inserter(testInput));
		//std::cout<< testInput << std::endl;
		EXPECT_NO_THROW((
			obj2 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj2->GetDataType(), DataTypes::Real);
		EXPECT_NEAR(obj1->GetReal(), obj2->GetReal(), 0.1e-100);
	}
	{
		std::unique_ptr<Json> obj1 = MakeUniquePtr::make_unique<RealType>(1);
		std::unique_ptr<Json> obj2;

		std::string testInput;
		obj1->ToString(std::back_inserter(testInput));
		//std::cout<< testInput << std::endl;

		EXPECT_NO_THROW((
			obj2 = Json::Parse(testInput.begin(), testInput.end())
		));
		EXPECT_EQ(obj2->GetDataType(), DataTypes::Real);
		EXPECT_NEAR(obj1->GetReal(), obj2->GetReal(), 0.1);
	}
}
