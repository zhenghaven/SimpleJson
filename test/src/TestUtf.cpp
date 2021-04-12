#include <cstring>

#include <gtest/gtest.h>

#include <SimpleJson/Internal/Utf.hpp>

#include "Utf8Map.hpp"
#include "Utf16Map.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
using namespace SimpleJson;
#else
using namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE;
#endif

namespace SimpleJson_Test
{
	extern size_t g_numOfTestFile;
}

GTEST_TEST(TestUtf, CountTestFile)
{
	++SimpleJson_Test::g_numOfTestFile;
}

GTEST_TEST(TestUtf, CalcNumBits)
{
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00000000), 0);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00000001), 1);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00000010), 2);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00000100), 3);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00001000), 4);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00010000), 5);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b00100000), 6);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b01000000), 7);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b10000000), 8);

	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0000000100000000), 9);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0000001000000000), 10);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0000010000000000), 11);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0000100000000000), 12);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0001000000000000), 13);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0010000000000000), 14);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b0100000000000000), 15);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b1000000000000000), 16);

	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b000000010000000000000000), 17);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b000000100000000000000000), 18);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b000001000000000000000000), 19);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b000010000000000000000000), 20);
	EXPECT_EQ(Internal::Internal::CalcNumOfBits<>(0b000100000000000000000000), 21);
}

GTEST_TEST(TestUtf, CalcNumContBytes)
{
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00000000), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00000001), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00000010), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00000100), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00001000), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00010000), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b00100000), 0);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b01000000), 0);

	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b10000000), 1);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0000000100000000), 1);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0000001000000000), 1);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0000010000000000), 1);

	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0000100000000000), 2);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0001000000000000), 2);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0010000000000000), 2);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b0100000000000000), 2);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b1000000000000000), 2);

	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b000000010000000000000000), 3);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b000000100000000000000000), 3);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b000001000000000000000000), 3);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b000010000000000000000000), 3);
	EXPECT_EQ(Internal::Internal::CalcUtf8NumContNeeded(0b000100000000000000000000), 3);

	EXPECT_THROW(Internal::Internal::CalcUtf8NumContNeeded(0xD800);, Internal::UtfConversionException);
	EXPECT_THROW(Internal::Internal::CalcUtf8NumContNeeded(0xDFFF);, Internal::UtfConversionException);
}

GTEST_TEST(TestUtf, CodePtToUtf8)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf8MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf8Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf8MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			const uint8_t* ptr = reinterpret_cast<const uint8_t*>(gk_Utf8Map[i][j]);
			size_t len = ptr[1] == 0 ? 1 : (ptr[2] == 0 ? 2 : (ptr[3] == 0 ? 3 : 4));
			std::string ref(ptr, ptr + len);
			char32_t codePt = begin + static_cast<char32_t>(j);

			std::string res;
			if (!Internal::Internal::IsValidCodePt(codePt))
			{
				EXPECT_THROW(Internal::CodePtToUtf8Once(codePt, std::back_inserter(res));, Internal::UtfConversionException);
			}
			else
			{
				Internal::CodePtToUtf8Once(codePt, std::back_inserter(res));

				if (res != ref)
				{
					++numErr;
				}

				EXPECT_EQ(ref, res);
			}
		}
	}
}

GTEST_TEST(TestUtf, Utf8ToCodePt)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf8MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf8Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf8MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				const uint8_t* ptr = reinterpret_cast<const uint8_t*>(gk_Utf8Map[i][j]);
				size_t len = ptr[1] == 0 ? 1 : (ptr[2] == 0 ? 2 : (ptr[3] == 0 ? 3 : 4));
				std::vector<uint16_t> utf8(ptr, ptr + len);

				char32_t resCodePt = 0;
				auto it = utf8.begin();
				std::tie(resCodePt, it) = Internal::Utf8ToCodePtOnce(utf8.begin(), utf8.end());

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf8MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				const uint8_t* ptr = reinterpret_cast<const uint8_t*>(gk_Utf8Map[i][j]);
				size_t len = ptr[1] == 0 ? 1 : (ptr[2] == 0 ? 2 : (ptr[3] == 0 ? 3 : 4));
				std::vector<int16_t> utf8(ptr, ptr + len);

				char32_t resCodePt = 0;
				auto it = utf8.begin();
				std::tie(resCodePt, it) = Internal::Utf8ToCodePtOnce(utf8.begin(), utf8.end());

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}
}

GTEST_TEST(TestUtf, CodePtToUtf16)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf16MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf16Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			const uint16_t* ptr = reinterpret_cast<const uint16_t*>(gk_Utf16Map[i][j]);
			size_t len = ptr[1] == 0 ? 1 : 2;
			std::u16string ref(ptr, ptr + len);
			char32_t codePt = begin + static_cast<char32_t>(j);

			std::u16string res;
			if (!Internal::Internal::IsValidCodePt(codePt))
			{
				EXPECT_THROW(Internal::CodePtToUtf16Once(codePt, std::back_inserter(res));, Internal::UtfConversionException);
			}
			else
			{
				Internal::CodePtToUtf16Once(codePt, std::back_inserter(res));

				if (res != ref)
				{
					++numErr;
				}

				EXPECT_EQ(ref, res);
			}
		}
	}
}

GTEST_TEST(TestUtf, Utf16ToCodePt)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf16MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf16Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				const uint16_t* ptr = reinterpret_cast<const uint16_t*>(gk_Utf16Map[i][j]);
				size_t len = ptr[1] == 0 ? 1 : 2;
				//std::vector<uint16_t> utf8(ptr, ptr + strLen);

				char32_t resCodePt = 0;
				auto it = ptr;
				std::tie(resCodePt, it) = Internal::Utf16ToCodePtOnce(ptr, ptr + len);

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				const uint16_t* ptr = reinterpret_cast<const uint16_t*>(gk_Utf16Map[i][j]);
				size_t len = ptr[1] == 0 ? 1 : 2;
				std::vector<int32_t> utf16(ptr, ptr + len);

				char32_t resCodePt = 0;
				auto it = utf16.begin();
				std::tie(resCodePt, it) = Internal::Utf16ToCodePtOnce(utf16.begin(), utf16.end());

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}
}

GTEST_TEST(TestUtf, CodePtToUtf32)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf16MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf16Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			std::u32string res;
			if (!Internal::Internal::IsValidCodePt(codePt))
			{
				EXPECT_THROW(Internal::CodePtToUtf32Once(codePt, std::back_inserter(res));, Internal::UtfConversionException);
			}
			else
			{
				Internal::CodePtToUtf32Once(codePt, std::back_inserter(res));

				if (res[0] != codePt)
				{
					++numErr;
				}

				EXPECT_EQ(codePt, res[0]);
			}
		}
	}
}

GTEST_TEST(TestUtf, Utf32ToCodePt)
{
	constexpr size_t maxNumErr = 10;
	constexpr size_t numMap = sizeof(gk_Utf16MapBegins) / sizeof(char32_t);
	constexpr size_t numPerMap = (sizeof(gk_Utf16Map) / (4 * sizeof(char))) / numMap;

	size_t numErr = 0;

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				uint64_t inArray[] = { codePt };
				char32_t resCodePt = 0;
				auto it = std::begin(inArray);
				std::tie(resCodePt, it) = Internal::Utf32ToCodePtOnce(std::begin(inArray), std::end(inArray));

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}

	for (size_t i = 0; i < numMap; ++i)
	{
		char32_t begin = gk_Utf16MapBegins[i];
		for (size_t j = 0; j < numPerMap && numErr < maxNumErr; ++j)
		{
			char32_t codePt = begin + static_cast<char32_t>(j);

			if (Internal::Internal::IsValidCodePt(codePt))
			{
				int64_t inArray[] = { codePt };
				char32_t resCodePt = 0;
				auto it = std::begin(inArray);
				std::tie(resCodePt, it) = Internal::Utf32ToCodePtOnce(std::begin(inArray), std::end(inArray));

				if (codePt != resCodePt)
				{
					++numErr;
				}
				EXPECT_EQ(codePt, resCodePt);
			}
		}
	}
}

GTEST_TEST(TestUtf, Conversion)
{
	{
		const char testUtf8 [] = "\xe6\xb5\x8b\xe8\xaf\x95\xe7\xa8\x8b\xe5\xba\x8f";
		const char16_t testUtf16 [] = {0x6d4b, 0x8bd5, 0x7a0b, 0x5e8f, 0};
		const char32_t testUtf32 [] = {0x00006d4b, 0x00008bd5, 0x00007a0b, 0x00005e8f, 0};

		auto testUtf8Str = std::string(testUtf8);
		auto testUtf16Str = std::u16string(testUtf16);
		auto testUtf32Str = std::u32string(testUtf32);
		{
			// === UTF-8 --> UTF-16
			auto utf16 = Internal::Utf8ToUtf16(testUtf8);
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			Internal::Utf8ToUtf16(testUtf8Str.begin(), testUtf8Str.end(), std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			auto utf16It = Internal::Utf8ToUtf16Once(std::begin(testUtf8) + 3, std::begin(testUtf8) + 6, std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str.substr(1, 1));
			EXPECT_EQ(utf16It, std::begin(testUtf8) + 6);

			// === UTF-8 --> UTF-32
			auto utf32 = Internal::Utf8ToUtf32(testUtf8);
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			Internal::Utf8ToUtf32(testUtf8Str.begin(), testUtf8Str.end(), std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			auto utf32It = Internal::Utf8ToUtf32Once(std::begin(testUtf8) + 3, std::begin(testUtf8) + 6, std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str.substr(1, 1));
			EXPECT_EQ(utf32It, std::begin(testUtf8) + 6);
		}
		{
			// === UTF-16 --> UTF-8
			auto utf8 = Internal::Utf16ToUtf8(testUtf16);
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			Internal::Utf16ToUtf8(testUtf16Str.begin(), testUtf16Str.end(), std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			auto utf8It = Internal::Utf16ToUtf8Once(std::begin(testUtf16) + 1, std::begin(testUtf16) + 2, std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str.substr(3, 3));
			EXPECT_EQ(utf8It, std::begin(testUtf16) + 2);

			// === UTF-16 --> UTF-32
			auto utf32 = Internal::Utf16ToUtf32(testUtf16);
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			Internal::Utf16ToUtf32(testUtf16Str.begin(), testUtf16Str.end(), std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			auto utf32It = Internal::Utf16ToUtf32Once(std::begin(testUtf16) + 1, std::begin(testUtf16) + 2, std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str.substr(1, 1));
			EXPECT_EQ(utf32It, std::begin(testUtf16) + 2);
		}
		{
			// === UTF-32 --> UTF-8
			auto utf8 = Internal::Utf32ToUtf8(testUtf32);
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			Internal::Utf32ToUtf8(testUtf32Str.begin(), testUtf32Str.end(), std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			auto utf8It = Internal::Utf32ToUtf8Once(std::begin(testUtf32) + 1, std::begin(testUtf32) + 2, std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str.substr(3, 3));
			EXPECT_EQ(utf8It, std::begin(testUtf32) + 2);

			// === UTF-32 --> UTF-16
			auto utf16 = Internal::Utf32ToUtf16(testUtf32);
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			Internal::Utf32ToUtf16(testUtf32Str.begin(), testUtf32Str.end(), std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			auto utf16It = Internal::Utf32ToUtf16Once(std::begin(testUtf32) + 1, std::begin(testUtf32) + 2, std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str.substr(1, 1));
			EXPECT_EQ(utf16It, std::begin(testUtf32) + 2);
		}
	}
	{
		const char testUtf8 [] = "\xF0\x9F\x98\x82 \xF0\x9F\x98\x86 \xE2\x9C\x85";
		const char16_t testUtf16 [] = {0xd83d, 0xde02, 0x0020, 0xd83d, 0xde06, 0x0020, 0x2705, 0};
		const char32_t testUtf32 [] = {0x0001f602, 0x00000020, 0x0001f606, 0x00000020, 0x00002705, 0};

		auto testUtf8Str = std::string(testUtf8);
		auto testUtf16Str = std::u16string(testUtf16);
		auto testUtf32Str = std::u32string(testUtf32);
		{
			// === UTF-8 --> UTF-16
			auto utf16 = Internal::Utf8ToUtf16(testUtf8);
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			Internal::Utf8ToUtf16(testUtf8Str.begin(), testUtf8Str.end(), std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			auto utf16It = Internal::Utf8ToUtf16Once(std::begin(testUtf8) + 5, std::begin(testUtf8) + 9, std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str.substr(3, 2));
			EXPECT_EQ(utf16It, std::begin(testUtf8) + 9);

			// === UTF-8 --> UTF-32
			auto utf32 = Internal::Utf8ToUtf32(testUtf8);
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			Internal::Utf8ToUtf32(testUtf8Str.begin(), testUtf8Str.end(), std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			auto utf32It = Internal::Utf8ToUtf32Once(std::begin(testUtf8) + 5, std::begin(testUtf8) + 9, std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str.substr(2, 1));
			EXPECT_EQ(utf32It, std::begin(testUtf8) + 9);
		}
		{
			// === UTF-16 --> UTF-8
			auto utf8 = Internal::Utf16ToUtf8(testUtf16);
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			Internal::Utf16ToUtf8(testUtf16Str.begin(), testUtf16Str.end(), std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			auto utf8It = Internal::Utf16ToUtf8Once(std::begin(testUtf16) + 3, std::begin(testUtf16) + 5, std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str.substr(5, 4));
			EXPECT_EQ(utf8It, std::begin(testUtf16) + 5);

			// === UTF-16 --> UTF-32
			auto utf32 = Internal::Utf16ToUtf32(testUtf16);
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			Internal::Utf16ToUtf32(testUtf16Str.begin(), testUtf16Str.end(), std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str);

			utf32.clear();
			EXPECT_EQ(utf32.size(), 0);
			auto utf32It = Internal::Utf16ToUtf32Once(std::begin(testUtf16) + 3, std::begin(testUtf16) + 5, std::back_inserter(utf32));
			EXPECT_EQ(utf32, testUtf32Str.substr(2, 1));
			EXPECT_EQ(utf32It, std::begin(testUtf16) + 5);
		}
		{
			// === UTF-32 --> UTF-8
			auto utf8 = Internal::Utf32ToUtf8(testUtf32);
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			Internal::Utf32ToUtf8(testUtf32Str.begin(), testUtf32Str.end(), std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str);

			utf8.clear();
			EXPECT_EQ(utf8.size(), 0);
			auto utf8It = Internal::Utf32ToUtf8Once(std::begin(testUtf32) + 2, std::begin(testUtf32) + 3, std::back_inserter(utf8));
			EXPECT_EQ(utf8, testUtf8Str.substr(5, 4));
			EXPECT_EQ(utf8It, std::begin(testUtf32) + 3);

			// === UTF-32 --> UTF-16
			auto utf16 = Internal::Utf32ToUtf16(testUtf32);
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			Internal::Utf32ToUtf16(testUtf32Str.begin(), testUtf32Str.end(), std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str);

			utf16.clear();
			EXPECT_EQ(utf16.size(), 0);
			auto utf16It = Internal::Utf32ToUtf16Once(std::begin(testUtf32) + 2, std::begin(testUtf32) + 3, std::back_inserter(utf16));
			EXPECT_EQ(utf16, testUtf16Str.substr(3, 2));
			EXPECT_EQ(utf16It, std::begin(testUtf32) + 3);
		}
	}
}
