// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>

#ifdef _MSC_VER
#include <windows.h>
#endif // _MSC_VER
#include <SimpleJson/InputStateMachine.hpp>
#include <SimpleJson/Internal/SimpleObjects.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

using namespace Internal::Obj;

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestInputStateMachine, CountTestFile)
{
	static auto tmp = ++SimpleJson_Test::g_numOfTestFile;
	(void)tmp;
}

GTEST_TEST(TestInputStateMachine, ParseErrorException)
{
	auto e = ParseError("Test ParseError", 123, 456);
	EXPECT_EQ(e.GetLineNum(), 123);
	EXPECT_EQ(e.GetColNum(), 456);
}

GTEST_TEST(TestInputStateMachine, Construction)
{
	std::string testInput = "test string";

	InputIteratorStateMachine<InIterator<char> > ism1(
		ToInIt(testInput.cbegin()));

	EXPECT_FALSE(ism1.HasEnd());
	EXPECT_FALSE(ism1.IsEnd());

	ForwardIteratorStateMachine<FrIterator<char, true> > ism2(
		ToFrIt<true>(testInput.cbegin()),
		ToFrIt<true>(testInput.cend()));

	EXPECT_TRUE(ism2.HasEnd());

	std::unique_ptr<InputStateMachineIf<char> > ism3(
		new InputIteratorStateMachine<InIterator<char> >(
			ToInIt(testInput.cbegin()))
	);
	ism3.reset();

	std::unique_ptr<InputStateMachineIf<char> > ism4(
		new ForwardIteratorStateMachine<FrIterator<char, true> >(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()))
	);
	ism4.reset();
}

GTEST_TEST(TestInputStateMachine, InputItBasicFunctional)
{
	using ISMType = InputIteratorStateMachine<InIterator<char> >;

	// skip all white space and count line number and col number
	{
		std::string testInput = "   \r  \n  \r\n  \r\r  \n\n  \n\r  xy";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'x');
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 2);
		EXPECT_EQ(ism1.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'x');
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 2);

		ISMType ism2(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism2.GetChar(), ' ');
		EXPECT_EQ(ism2.GetLineCount(), 0);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.SkipSpaceAndGetCharAndAdv(), 'x');
		EXPECT_EQ(ism2.GetChar(), 'y');
		EXPECT_EQ(ism2.GetLineCount(), 9);
		EXPECT_EQ(ism2.GetColCount(), 3);
	}

	// advance one by one
	{
		std::string testInput = "\r\r\n\n\rx";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\n'); // on line 1, col 1
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 1);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\n');
		EXPECT_EQ(ism1.GetChar(), '\n'); // on line 2, col 0
		EXPECT_EQ(ism1.GetLineCount(), 2);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\n');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 3, col 0
		EXPECT_EQ(ism1.GetLineCount(), 3);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 0);
	}

	// Mix
	{
		std::string testInput = "\r\r\n\n\rx";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 0);

		ISMType ism2(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism2.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism2.GetLineCount(), 0);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism2.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism2.GetLineCount(), 1);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism2.GetChar(), '\n'); // on line 1, col 1
		EXPECT_EQ(ism2.GetLineCount(), 1);
		EXPECT_EQ(ism2.GetColCount(), 1);

		EXPECT_EQ(ism2.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism2.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism2.GetLineCount(), 4);
		EXPECT_EQ(ism2.GetColCount(), 0);
	}
}

GTEST_TEST(TestInputStateMachine, ForwardItBasicFunctional)
{
	using ISMType = ForwardIteratorStateMachine<FrIterator<char, true> >;

	// skip all white space and count line number and col number
	{
		std::string testInput = "   \r  \n  \r\n  \r\r  \n\n  \n\r  xy";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'x');
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 2);
		EXPECT_EQ(ism1.SkipSpaceAndGetCharAndAdv(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'y');
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 3);
		EXPECT_EQ(ism1.SkipSpaceAndGetCharAndAdv(), 'y');
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 4);

		ISMType ism2(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism2.GetChar(), ' ');
		EXPECT_EQ(ism2.GetLineCount(), 0);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.SkipSpaceAndGetCharAndAdv(), 'x');
		EXPECT_EQ(ism2.GetChar(), 'y');
		EXPECT_EQ(ism2.GetLineCount(), 9);
		EXPECT_EQ(ism2.GetColCount(), 3);
		EXPECT_EQ(ism2.SkipSpaceAndGetCharAndAdv(), 'y');
		EXPECT_THROW(ism2.GetChar(), ParseError);
		EXPECT_EQ(ism2.GetLineCount(), 9);
		EXPECT_EQ(ism2.GetColCount(), 4);
	}

	// skip all white space and reach the end
	{
		std::string testInput = "   \r  \n  \r\n  \r\r  \n\n  \n\r  ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.SkipSpaceAndGetChar(), ParseError);
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 9);
		EXPECT_EQ(ism1.GetColCount(), 2);

		ISMType ism2(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism2.GetChar(), ' ');
		EXPECT_EQ(ism2.GetLineCount(), 0);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_THROW(ism2.SkipSpaceAndGetCharAndAdv(), ParseError);
		EXPECT_THROW(ism2.GetChar(), ParseError);
		EXPECT_EQ(ism2.GetLineCount(), 9);
		EXPECT_EQ(ism2.GetColCount(), 2);
	}

	// advance one by one
	{
		std::string testInput = "\r\r\n\n\rx";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\n'); // on line 1, col 1
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 1);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\n');
		EXPECT_EQ(ism1.GetChar(), '\n'); // on line 2, col 0
		EXPECT_EQ(ism1.GetLineCount(), 2);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\n');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 3, col 0
		EXPECT_EQ(ism1.GetLineCount(), 3);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), 'x');
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 1);

		EXPECT_THROW(ism1.GetCharAndAdv(), ParseError);
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}

	// Mix
	{
		std::string testInput = "\r\r\n\n\rx";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism1.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism1.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_EQ(ism1.GetCharAndAdv(), 'x');
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 1);

		EXPECT_THROW(ism1.GetCharAndAdv(), ParseError);
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 4);
		EXPECT_EQ(ism1.GetColCount(), 1);

		ISMType ism2(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism2.GetChar(), '\r'); // on line 0, col 0
		EXPECT_EQ(ism2.GetLineCount(), 0);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism2.GetChar(), '\r'); // on line 1, col 0
		EXPECT_EQ(ism2.GetLineCount(), 1);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.GetCharAndAdv(), '\r');
		EXPECT_EQ(ism2.GetChar(), '\n'); // on line 1, col 1
		EXPECT_EQ(ism2.GetLineCount(), 1);
		EXPECT_EQ(ism2.GetColCount(), 1);

		EXPECT_EQ(ism2.SkipSpaceAndGetChar(), 'x');
		EXPECT_EQ(ism2.GetChar(), 'x'); // on line 4, col 0
		EXPECT_EQ(ism2.GetLineCount(), 4);
		EXPECT_EQ(ism2.GetColCount(), 0);

		EXPECT_EQ(ism2.GetCharAndAdv(), 'x');
		EXPECT_THROW(ism2.GetChar(), ParseError);
		EXPECT_EQ(ism2.GetLineCount(), 4);
		EXPECT_EQ(ism2.GetColCount(), 1);

		EXPECT_THROW(ism2.GetCharAndAdv(), ParseError);
		EXPECT_THROW(ism2.GetChar(), ParseError);
		EXPECT_EQ(ism2.GetLineCount(), 4);
		EXPECT_EQ(ism2.GetColCount(), 1);

		auto testProg3 = [testInput](){
			ISMType ism3(
				ToFrIt<true>(testInput.cbegin()),
				ToFrIt<true>(testInput.cbegin()));
		};
		EXPECT_THROW(testProg3(), ParseError);
	}
}

GTEST_TEST(TestInputStateMachine, InputItExpDelimiter)
{
	using ISMType = InputIteratorStateMachine<InIterator<char> >;

	// pass
	{
		std::string testInput = "   \n  : ";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_NO_THROW(ism1.ExpDelimiter(':'));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 3);
	}
	{
		std::string testInput = ": ";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), ':');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_NO_THROW(ism1.ExpDelimiter(':'));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}

	// fail
	{
		std::string testInput = "   \n  : ";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.ExpDelimiter(','), ParseError);
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 3);
	}
	{
		std::string testInput = ": ";
		ISMType ism1(ToInIt(testInput.cbegin()));
		EXPECT_EQ(ism1.GetChar(), ':');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.ExpDelimiter(','), ParseError);
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}
}

GTEST_TEST(TestInputStateMachine, ForwardItExpDelimiter)
{
	using ISMType = ForwardIteratorStateMachine<FrIterator<char, true> >;

	// pass
	{
		std::string testInput = "   \n  : ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_NO_THROW(ism1.ExpDelimiter(':'));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 3);
	}
	{
		std::string testInput = ": ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ':');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_NO_THROW(ism1.ExpDelimiter(':'));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}

	// fail
	{
		std::string testInput = "   \n  : ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.ExpDelimiter(','), ParseError);
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 1);
		EXPECT_EQ(ism1.GetColCount(), 3);
	}
	{
		std::string testInput = ": ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ':');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.ExpDelimiter(','), ParseError);
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}
	{
		std::string testInput = " ";
		ISMType ism1(
			ToFrIt<true>(testInput.cbegin()),
			ToFrIt<true>(testInput.cend()));
		EXPECT_EQ(ism1.GetChar(), ' ');
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 0);

		EXPECT_THROW(ism1.ExpDelimiter(','), ParseError);
		EXPECT_THROW(ism1.GetChar(), ParseError);
		EXPECT_EQ(ism1.GetLineCount(), 0);
		EXPECT_EQ(ism1.GetColCount(), 1);
	}
}