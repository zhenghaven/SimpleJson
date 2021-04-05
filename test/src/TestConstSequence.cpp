#include <gtest/gtest.h>

#include <SimpleJson/ConstSequence.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestStaticSequence, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestStaticSequence, UInt64Array)
{
	constexpr uint64_t testCArray[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	std::array<uint64_t, sizeof(testCArray)/sizeof(uint64_t)> testArray;
	std::copy(std::begin(testCArray), std::end(testCArray), testArray.begin());

	static_assert(SIMPLEJSON_CONST_ARRAY(testCArray)::sk_size == 10, "StaticSequence Impl Error!");

	auto outData = SIMPLEJSON_CONST_ARRAY(testCArray)::GetData();
	EXPECT_EQ(outData, testArray);
}

GTEST_TEST(TestStaticSequence, String)
{
	static_assert(SIMPLEJSON_CONST_STRING("Test_string_1")::sk_size == 13, "StaticSequence Impl Error!");

	auto outStr = SIMPLEJSON_CONST_STRING("Test_string_1")::GetStr();
	EXPECT_EQ(outStr, "Test_string_1");
}
