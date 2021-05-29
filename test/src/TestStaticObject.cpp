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

GTEST_TEST(TestStaticObject, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestStaticObject, Constrcution)
{
	{
		using KVMap = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), Integer<> >,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), Real<> >
		>::type;
		StaticObject<KVMap> sobj;
		sobj.Get<SIMPLEJSON_CONST_STRING("Key_1")>() = 1234567;
		sobj.Get<SIMPLEJSON_CONST_STRING("Key_2")>() = 1.0;

		const auto& csobj = sobj;
		const auto& key1val = csobj.Get<SIMPLEJSON_CONST_STRING("Key_1")>();
		EXPECT_EQ(key1val, 1234567);
	}
}
