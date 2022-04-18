// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

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

GTEST_TEST(TestSpecialParser, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

namespace
{

// {
//   "color": "red" | "green" | "blue",
//   "pos" : {"x": x, "y": y} | [x, y]
// }

static constexpr char sk_testInput_v01[] =
"{\n\
	\"color\": \"red\",\n\
	\"pos\": {\"x\":1, \"y\":2}\n\
}";
static constexpr char sk_testInput_v02[] =
"{\n\
	\"color\": \"green\",\n\
	\"pos\": [1, 2]\n\
}";
static constexpr char sk_testInput_i01[] =
"{\n\
	\"color\": \"rgb\",\n\
	\"pos\": [1, 2]\n\
}";
static constexpr char sk_testInput_i02[] =
"{\n\
	\"color\": \"green\",\n\
	\"pos\": \"(1,2)\"\n\
}";

// LCOV_EXCL_START
/**
 * @brief A special string parser that has a list of possible values
 *
 */
class TestLimitedStringParser : public StringParser
{
public: // static members

	using Base = StringParser;

public:

	using Base::StringParserImpl;

	virtual ~TestLimitedStringParser() = default;

	void AppendOption(const Internal::Obj::String& opt)
	{
		m_list.push_back(opt);
	}

	using Base::Parse;

	virtual RetType Parse(InputStateMachineIf<InputChType>& ism) const override
	{
		auto res = Base::Parse(ism);

		if (std::find(m_list.begin(), m_list.end(), res) == m_list.end())
		{
			throw std::runtime_error("The parsed value is not allowed");
		}

		return res;
	}

private:

	std::vector<Internal::Obj::String> m_list;

}; // class TestLimitedStringParser


/**
 * @brief A special generic object parser that can disable some parser types
 *
 */
class TestGenericObjectParser : public GenericObjectParser
{
public: // static members

	using Base = GenericObjectParser;

public:

	TestGenericObjectParser():
		Base::GenericObjectParserImpl(),
		m_nullParserSwitch(false),
		m_boolParserSwitch(false),
		m_numberParserSwitch(false),
		m_stringParserSwitch(false),
		m_listParserSwitch(false),
		m_dictParserSwitch(false)
	{}

	virtual ~TestGenericObjectParser() = default;


	virtual const NullParser* GetNullParser() const override
	{
		if(!m_nullParserSwitch)
		{
			throw std::runtime_error("Null parser is disabled");
		}
		return Base::GetNullParser();
	}

	virtual const BoolParser* GetBoolParser() const override
	{
		if(!m_boolParserSwitch)
		{
			throw std::runtime_error("Bool parser is disabled");
		}
		return Base::GetBoolParser();
	}

	virtual const GenericNumberParser* GetNumberParser() const override
	{
		if(!m_numberParserSwitch)
		{
			throw std::runtime_error("Number parser is disabled");
		}
		return Base::GetNumberParser();
	}

	virtual const StringParser* GetStringParser() const override
	{
		if(!m_stringParserSwitch)
		{
			throw std::runtime_error("String parser is disabled");
		}
		return Base::GetStringParser();
	}

	virtual const ListParser* GetListParser() const override
	{
		if(!m_listParserSwitch)
		{
			throw std::runtime_error("List parser is disabled");
		}
		return Base::GetListParser();
	}

	virtual const DictParser* GetDictParser() const override
	{
		if(!m_dictParserSwitch)
		{
			throw std::runtime_error("Dict parser is disabled");
		}
		return Base::GetDictParser();
	}

	virtual void SetStringParser(StringParserPtr p) override
	{
		m_stringParserSwitch = true;
		Base::SetStringParser(std::move(p));
	}

	virtual void SetListParser(ListParserPtr p) override
	{
		m_listParserSwitch = true;
		Base::SetListParser(std::move(p));
	}

	virtual void SetDictParser(DictParserPtr p) override
	{
		m_dictParserSwitch = true;
		Base::DictParserPtr(std::move(p));
	}

private:
	bool m_nullParserSwitch;
	bool m_boolParserSwitch;
	bool m_numberParserSwitch;
	bool m_stringParserSwitch;
	bool m_listParserSwitch;
	bool m_dictParserSwitch;

}; // class TestGenericObjectParser

/**
 * @brief A special dict parser that can assign different parser according to
 *        the key
 *
 */
class TestSpecDictParser : public DictParserT<GenericObjectParser>
{
public: // static members

	using Base = DictParserT<GenericObjectParser>;

public:

	using Base::DictParserImpl;

	virtual ~TestSpecDictParser() = default;

	virtual void SetParser(
		const typename Base::KeyParser::RetType& key,
		std::unique_ptr<GenericObjectParser> p)
	{
		m_map[key] = std::move(p);
	}

	virtual const ValParser* GetValParser(
		const typename KeyParser::RetType& k) const override
	{
		auto it = m_map.find(k);
		if (it != m_map.end())
		{
			return it->second.get();
		}
		return Base::GetValParser(k);
	}

private:

	std::unordered_map<typename Base::KeyParser::RetType,
		std::unique_ptr<GenericObjectParser> > m_map;

}; // class TestSpecDictParser

// LCOV_EXCL_STOP

}

GTEST_TEST(TestSpecialParser, Construct)
{
	TestLimitedStringParser strParser;
	TestGenericObjectParser objParser;
	TestSpecDictParser dictParser;
}

GTEST_TEST(TestSpecialParser, SpecParse)
{
	TestSpecDictParser dictParser;

	auto colorStrParser = Internal::make_unique<TestLimitedStringParser>();
	colorStrParser->AppendOption(Internal::Obj::String("red"));
	colorStrParser->AppendOption(Internal::Obj::String("green"));
	colorStrParser->AppendOption(Internal::Obj::String("blue"));
	auto colorParser = Internal::make_unique<TestGenericObjectParser>();
	colorParser->SetStringParser(std::move(colorStrParser));

	auto posParser = Internal::make_unique<TestGenericObjectParser>();
	posParser->SetDictParser(
		Internal::make_unique<DictParserT<GenericObjectParser> >());
	posParser->SetListParser(
		Internal::make_unique<ListParserT<GenericObjectParser> >());

	dictParser.SetParser(
		Internal::Obj::String("color"),
		std::move(colorParser));
	dictParser.SetParser(
		Internal::Obj::String("pos"),
		std::move(posParser));

	GenericObjectParser genericParser;

	// Valid input
	{
		auto obj1 = dictParser.Parse(sk_testInput_v01);
		auto obj2 = genericParser.Parse(sk_testInput_v01);
		EXPECT_EQ(obj1, obj2);
	}
	{
		auto obj1 = dictParser.Parse(sk_testInput_v02);
		auto obj2 = genericParser.Parse(sk_testInput_v02);
		EXPECT_EQ(obj1, obj2);
	}

	// Invalid input
	{
		EXPECT_THROW(
			dictParser.Parse(sk_testInput_i01),
			std::runtime_error);
		EXPECT_NO_THROW(
			genericParser.Parse(sk_testInput_i01));
	}
	{
		EXPECT_THROW(
			dictParser.Parse(sk_testInput_i02),
			std::runtime_error);
		EXPECT_NO_THROW(
			genericParser.Parse(sk_testInput_i02));
	}
}
