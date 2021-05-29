#include <gtest/gtest.h>

#include <SimpleJson/ConstSequence.hpp>
#include <SimpleJson/ConstMap.hpp>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestConstMap, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestConstMap, BuildConstMap)
{
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(Map::sk_size == 5, "Const Map implementation error.");
	}
}

GTEST_TEST(TestConstMap, ConstMapFindVal)
{
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_1")> , uint8_t>::value, "Const Map implementation error.");
		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_2")> , uint16_t>::value, "Const Map implementation error.");
		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_3")> , uint32_t>::value, "Const Map implementation error.");
		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_4")> , uint64_t>::value, "Const Map implementation error.");
		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_5")> , float>::value, "Const Map implementation error.");
		static_assert(std::is_same<Map::FindValType<SIMPLEJSON_CONST_STRING("Key_6")> , ConstMapError>::value, "Const Map implementation error.");
	}
}

GTEST_TEST(TestConstMap, ConstMapHasKey)
{
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(Map::HasKey<SIMPLEJSON_CONST_STRING("Key_1")>::value, "Const Map implementation error.");
		static_assert(Map::HasKey<SIMPLEJSON_CONST_STRING("Key_2")>::value, "Const Map implementation error.");
		static_assert(Map::HasKey<SIMPLEJSON_CONST_STRING("Key_3")>::value, "Const Map implementation error.");
		static_assert(Map::HasKey<SIMPLEJSON_CONST_STRING("Key_4")>::value, "Const Map implementation error.");
		static_assert(Map::HasKey<SIMPLEJSON_CONST_STRING("Key_5")>::value, "Const Map implementation error.");
		static_assert(!Map::HasKey<SIMPLEJSON_CONST_STRING("Key_6")>::value, "Const Map implementation error.");
	}
}

GTEST_TEST(TestConstMap, ConstMapHasDupKey)
{
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(!Map::HasDupKey::value, "Const Map implementation error.");
	}
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(Map::HasDupKey::value, "Const Map implementation error.");
	}
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(Map::HasDupKey::value, "Const Map implementation error.");
	}
	{
		using Map = typename BuildConstMap<
			ConstPair<SIMPLEJSON_CONST_STRING("Key_1"), uint8_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_2"), uint16_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_3"), uint32_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_4"), uint64_t>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>,
			ConstPair<SIMPLEJSON_CONST_STRING("Key_5"), float>
		>::type;

		static_assert(Map::HasDupKey::value, "Const Map implementation error.");
	}
}
