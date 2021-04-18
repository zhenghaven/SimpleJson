#pragma once

#include <cuchar>

#include <string>
#include <iterator>
#include <type_traits>

#include "../Exceptions.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Internal
	{

		/**
		 * @brief An exception class that is thrown when the UTF conversion failed.
		 *
		 */
		class UtfConversionException : public Exception
		{
		public:

			/**
			 * @brief Construct a new UTF conversion Exception object
			 *
			 * @param what_arg explanatory string
			 */
			UtfConversionException(const char* what_arg) :
				Exception(what_arg)
			{}

			/**
			 * @brief Construct a new UTF conversion Exception object
			 *
			 * @param what_arg explanatory string
			 */
			UtfConversionException(const std::string& what_arg) :
				Exception(what_arg)
			{}

			UtfConversionException(const UtfConversionException& other) noexcept :
				Exception(other)
			{}

			/**
			 * @brief Destroy the UTF conversion Exception object
			 *
			 */
			virtual ~UtfConversionException()
			{}
		};

		namespace Internal
		{
			template<bool B, class T = void>
			using EnableIfT = typename std::enable_if<B,T>::type;

#if __cplusplus < 201703L
			template<class F, class... ArgTypes>
			using InvokeResult = std::result_of<F(ArgTypes...)>;
#else
			template<class F, class... ArgTypes>
			using InvokeResult = std::invoke_result<F, ArgTypes...>;
#endif

			template<class F, class... ArgTypes>
			using InvokeResultT = typename InvokeResult<F, ArgTypes...>::type;
		}

		namespace Internal
		{
			inline bool IsReservedCodePt(char32_t val)
			{
				return (0xD800 <= val) && (val <= 0xDFFF);
			}

			inline bool IsValidCodePt(char32_t val)
			{
				return (val <= 0x10FFFF) && !IsReservedCodePt(val);
			}

			template<size_t _Max = 21>
			inline size_t CalcNumOfBits(char32_t val)
			{
				constexpr char32_t maxBit = 1 << (_Max - 1);
				char32_t testBit = maxBit;
				size_t res = 21;

				while(testBit > 0 && !(val & testBit))
				{
					testBit >>= 1;
					--res;
				}

				return res;
			}
		}

		namespace Internal
		{
			template<typename ValType, size_t _Bytes,
				EnableIfT<_Bytes <= sizeof(ValType), int> = 0>
			constexpr ValType TrailingOnes()
			{
				return sizeof(ValType) == _Bytes ?
					static_cast<ValType>(~static_cast<ValType>(ValType(0x00))) :
					(
						_Bytes == 1 ? static_cast<ValType>(ValType(static_cast< uint8_t>(0xFFU))) : (
						_Bytes == 2 ? static_cast<ValType>(ValType(static_cast<uint16_t>(0xFFFFU))) :	(
						_Bytes == 3 ? static_cast<ValType>(ValType(static_cast<uint32_t>(0xFFFFFFU))) : (
						_Bytes == 4 ? static_cast<ValType>(ValType(static_cast<uint32_t>(0xFFFFFFFFU))) : (
						_Bytes == 5 ? static_cast<ValType>(ValType(static_cast<uint64_t>(0xFFFFFFFFFFULL))) : (
						_Bytes == 6 ? static_cast<ValType>(ValType(static_cast<uint64_t>(0xFFFFFFFFFFFFULL))) : (
						_Bytes == 7 ? static_cast<ValType>(ValType(static_cast<uint64_t>(0xFFFFFFFFFFFFFFULL))) : (
							throw std::invalid_argument("Invalid template argument")
						)))))))
					)
					;
			}
			static_assert(TrailingOnes<uint8_t, 1>() == 0xFF, "Programming Error");
			static_assert(TrailingOnes< int8_t, 1>() == ~static_cast<int8_t>(0x00), "Programming Error");
			static_assert(TrailingOnes<uint16_t, 1>() == 0x00FF, "Programming Error");
			static_assert(TrailingOnes< int16_t, 1>() == 0x00FF, "Programming Error");
			static_assert(TrailingOnes<uint32_t, 1>() == 0x000000FF, "Programming Error");
			static_assert(TrailingOnes< int32_t, 1>() == 0x000000FF, "Programming Error");
			static_assert(TrailingOnes<uint16_t, 2>() == 0xFFFF, "Programming Error");
			static_assert(TrailingOnes< int16_t, 2>() == ~static_cast<int16_t>(0x00), "Programming Error");
			static_assert(TrailingOnes<uint32_t, 2>() == 0x0000FFFF, "Programming Error");
			static_assert(TrailingOnes< int32_t, 2>() == 0x0000FFFF, "Programming Error");
			static_assert(TrailingOnes<uint32_t, 3>() == 0x00FFFFFF, "Programming Error");
			static_assert(TrailingOnes< int32_t, 3>() == 0x00FFFFFF, "Programming Error");
			static_assert(TrailingOnes<uint32_t, 4>() == 0xFFFFFFFF, "Programming Error");
			static_assert(TrailingOnes< int32_t, 4>() == ~static_cast<int32_t>(0x00), "Programming Error");

			template<typename ValType, size_t _Bytes,
				EnableIfT<_Bytes <= sizeof(ValType), int> = 0>
			constexpr ValType TrailingZeros()
			{
				return
					static_cast<ValType>(
						~static_cast<ValType>(TrailingOnes<ValType, _Bytes>()
						) // 0x0000FFFF...
					) // 0xFFFF0000...
					;
			}
			static_assert(TrailingZeros< uint8_t, 1>() == 0x00, "Programming Error");
			static_assert(TrailingZeros<  int8_t, 1>() == 0x00, "Programming Error");
			static_assert(TrailingZeros<uint16_t, 1>() == 0xFF00, "Programming Error");
			static_assert(TrailingZeros< int16_t, 1>() == ~static_cast<int16_t>(0x00FF), "Programming Error");
			static_assert(TrailingZeros<uint32_t, 1>() == 0xFFFFFF00U, "Programming Error");
			static_assert(TrailingZeros< int32_t, 1>() == ~static_cast<int32_t>(0x00FF), "Programming Error");
			static_assert(TrailingZeros<uint64_t, 1>() == 0xFFFFFFFFFFFFFF00ULL, "Programming Error");
			static_assert(TrailingZeros< int64_t, 1>() == ~static_cast<int64_t>(0x00FF), "Programming Error");
			static_assert(TrailingZeros<uint16_t, 2>() == 0x0000, "Programming Error");
			static_assert(TrailingZeros< int16_t, 2>() == 0x0000, "Programming Error");
			static_assert(TrailingZeros<uint32_t, 2>() == 0xFFFF0000U, "Programming Error");
			static_assert(TrailingZeros< int32_t, 2>() == ~static_cast<int32_t>(0x00FFFF), "Programming Error");
			static_assert(TrailingZeros<uint64_t, 2>() == 0xFFFFFFFFFFFF0000ULL, "Programming Error");
			static_assert(TrailingZeros< int64_t, 2>() == ~static_cast<int64_t>(0x00FFFF), "Programming Error");
			static_assert(TrailingZeros<uint32_t, 3>() == 0xFF000000U, "Programming Error");
			static_assert(TrailingZeros< int32_t, 3>() == ~static_cast<int32_t>(0x00FFFFFF), "Programming Error");
			static_assert(TrailingZeros<uint64_t, 3>() == 0xFFFFFFFFFF000000ULL, "Programming Error");
			static_assert(TrailingZeros< int64_t, 3>() == ~static_cast<int64_t>(0x00FFFFFF), "Programming Error");
			static_assert(TrailingZeros<uint32_t, 4>() == 0x00000000U, "Programming Error");
			static_assert(TrailingZeros< int32_t, 4>() == 0x00000000U, "Programming Error");
			static_assert(TrailingZeros<uint64_t, 4>() == 0xFFFFFFFF00000000ULL, "Programming Error");
			static_assert(TrailingZeros< int64_t, 4>() == ~static_cast<int64_t>(0x00FFFFFFFF), "Programming Error");
		}

		inline bool IsValidUtfCodePt(char32_t val)
		{
			return Internal::IsValidCodePt(val);
		}

		template<typename ValType>
		inline constexpr bool IsAscii(ValType val)
		{
			return (~((~val) | 0x7F)) == 0;
		}
		static_assert(IsAscii('\0'), "Programming Error");
		static_assert(IsAscii('\n'), "Programming Error");
		static_assert(IsAscii('a'), "Programming Error");
		static_assert(IsAscii('\x7f'), "Programming Error");
		static_assert(!IsAscii('\x80'), "Programming Error");
		static_assert(!IsAscii(~static_cast<int8_t>(0)), "Programming Error");

		template<typename ValType>
		inline constexpr bool IsPrintableAscii(ValType val)
		{
			return IsAscii(val) && (0x20 <= static_cast<uint8_t>(val) && static_cast<uint8_t>(val) <= 0x7E);
		}
		static_assert(IsPrintableAscii('a'), "Programming Error");
		static_assert(IsPrintableAscii(' '), "Programming Error");
		static_assert(IsPrintableAscii('~'), "Programming Error");
		static_assert(!IsPrintableAscii('\0'), "Programming Error");
		static_assert(!IsPrintableAscii('\n'), "Programming Error");
		static_assert(!IsPrintableAscii('\x7f'), "Programming Error");
		static_assert(!IsPrintableAscii('\x80'), "Programming Error");

		namespace Internal
		{
			inline size_t CalcUtf8NumContNeeded(char32_t val)
			{
				if (!IsValidCodePt(val))
				{
					throw UtfConversionException("Invalid UTF Code Point" " - "
						+ std::to_string(val) + " is not a valid UTF code point.");
				}

				switch (CalcNumOfBits<>(val))
				{
					case 0:
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
						return 0;
					case 8:
					case 9:
					case 10:
					case 11:
						return 1;
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
						return 2;
					default:
						return 3;
				}
			}

			template<typename ValType,
				EnableIfT<sizeof(ValType) >= sizeof(uint8_t), int> = 0>
			inline std::pair<size_t, uint8_t> Utf8ReadLeading(const ValType& val)
			{
				if (val & TrailingZeros<ValType, 1>())
				{
					throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-8 leading byte.");
				}

				// Case 1: 1 byte - (0) ~ (7) bits
				if (!(val & 0b10000000))
				{
					size_t numCont = 0;
					uint8_t res = static_cast<uint8_t>(val & 0b01111111);
					return std::make_pair(numCont, res);
				}
				// Case 2: 2 bytes - (2 + 6 = 8) ~ (5 + 6 = 11) bits
				else if ((val & 0b10000000) && (val & 0b01000000) && !(val & 0b00100000) &&
					(val & 0b00011110))
				{
					size_t numCont = 1;
					uint8_t res = static_cast<uint8_t>(val & 0b00011111);
					return std::make_pair(numCont, res);
				}
				// Case 2: 3 bytes - (0 + 6 + 6 = 12) ~ (4 + 6 + 6 = 16) bits
				else if ((val & 0b10000000) && (val & 0b01000000) && (val & 0b00100000) && !(val & 0b00010000))
				{
					size_t numCont = 2;
					uint8_t res = static_cast<uint8_t>(val & 0b00001111);
					return std::make_pair(numCont, res);
				}
				// Case 2: 4 bytes - (0 + 5 + 6 + 6 = 17) ~ (3 + 6 + 6 + 6 = 21) bits
				else if ((val & 0b10000000) && (val & 0b01000000) && (val & 0b00100000) && (val & 0b00010000) && !(val & 0b00001000))
				{
					size_t numCont = 3;
					uint8_t res = static_cast<uint8_t>(val & 0b00000111);
					return std::make_pair(numCont, res);
				}
				else
				{
					throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-8 leading byte.");
				}
			}

			template<typename ValType,
				EnableIfT<sizeof(ValType) >= sizeof(uint8_t), int> = 0>
			inline uint8_t Utf8ReadCont(const ValType& val)
			{
				if (!(val & TrailingZeros<ValType, 1>()) && (val & 0b10000000) && !(val & 0b01000000))
				{
					uint8_t res = static_cast<uint8_t>(val & 0b00111111);
					return res;
				}
				else
				{
					throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-8 continuation byte.");
				}
			}

			template<typename ValType>
			inline bool IsUtf16Surrogate(const ValType& val)
			{
				return (val & 0b1000000000000000) && (val & 0b0100000000000000) &&
					!(val & 0b0010000000000000) && (val & 0b0001000000000000) &&
					(val & 0b0000100000000000);
			}

			template<typename ValType>
			inline bool IsUtf16SurrogateFirst(const ValType& val)
			{
				return IsUtf16Surrogate(val) &&
					!(val & 0b0000010000000000);
			}

			template<typename ValType>
			inline bool IsUtf16SurrogateSecond(const ValType& val)
			{
				return IsUtf16Surrogate(val) &&
					(val & 0b0000010000000000);
			}
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline std::pair<char32_t, InputIt> Utf8ToCodePtOnce(InputIt begin, InputIt end)
		{
			if (begin == end)
			{
				throw UtfConversionException("Unexpected Ending" " - "
					"String ends unexpected while reading the next UTF-8 char.");
			}

			char32_t res = 0;

			uint8_t leading = 0;
			size_t numCont = 0;
			std::tie(numCont, leading) = Internal::Utf8ReadLeading(*begin);
			++begin;
			res |= leading;

			for (size_t i = 0; i < numCont; ++i)
			{
				uint8_t b = Internal::Utf8ReadCont(*begin);
				++begin;

				res <<= 6;
				res |= b;

				if (i == 0)
				{
					if ((numCont == 2 && res < 0b00100000) ||
						(numCont == 3 && res < 0b00010000))
					{
						throw UtfConversionException("Invalid Encoding" " - "
							"Invalid UTF-8 continuation byte.");
					}
				}
			}

			if (!Internal::IsValidCodePt(res))
			{
				throw UtfConversionException("Invalid Code Point" " - "
					"The code point read from the given UTF-8 encoding is invalid.");
			}

			return std::make_pair(res, begin);
		}

		template<typename OutputIt>
		inline void CodePtToUtf8Once(char32_t val, OutputIt oit)
		{
			size_t numCont = Internal::CalcUtf8NumContNeeded(val);
			char res[4]{0, 0, 0, 0};

			for (size_t i = numCont; i > 0; --i)
			{
				res[i] |= 0b10000000;
				res[i] |= static_cast<char>((val & 0b00111111));

				val >>= 6;
			}

			switch (numCont)
			{
			case 0:
				res[0] |= static_cast<char>((val & 0b01111111));
				break;
			case 1:
				res[0] |= 0b11000000;
				res[0] |= static_cast<char>((val & 0b00011111));
				break;
			case 2:
				res[0] |= 0b11100000;
				res[0] |= static_cast<char>((val & 0b00001111));
				break;
			case 3:
			default:
				res[0] |= 0b11110000;
				res[0] |= static_cast<char>((val & 0b00000111));
				break;
			}

			std::copy(std::begin(res), std::begin(res) + 1 + numCont, oit);
		}

		inline size_t CodePtToUtf8OnceGetSize(char32_t val)
		{
			return 1 + Internal::CalcUtf8NumContNeeded(val);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline std::pair<char32_t, InputIt> Utf16ToCodePtOnce(InputIt begin, InputIt end)
		{
			using ValueType = typename std::iterator_traits<InputIt>::value_type;

			if (begin == end)
			{
				throw UtfConversionException("Unexpected Ending" " - "
					"String ends unexpected while reading the next UTF-16 bytes.");
			}
			if (*begin & Internal::TrailingZeros<ValueType, 2>())
			{
				throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-16 leading bytes.");
			}

			uint16_t first = static_cast<uint16_t>(*begin & 0xFFFF);
			++begin;

			if (Internal::IsUtf16SurrogateFirst(first))
			// Surrogate Pairs
			{
				if (begin == end)
				{
					throw UtfConversionException("Unexpected Ending" " - "
						"String ends unexpected while reading the next UTF-16 bytes.");
				}
				if (*begin & Internal::TrailingZeros<ValueType, 2>())
				{
					throw UtfConversionException("Invalid Encoding" " - "
						"Invalid UTF-16 continuation bytes.");
				}

				uint16_t second = static_cast<uint16_t>(*begin & 0xFFFF);
				++begin;

				if (Internal::IsUtf16SurrogateSecond(second))
				{
					char32_t res = 0x10000;
					res += static_cast<char32_t>(first & 0x03FF) << 10;
					res += static_cast<char32_t>(second & 0x03FF);

					if (!Internal::IsValidCodePt(res))
					{
						throw UtfConversionException("Invalid Code Point" " - "
							"The code point read from the given UTF-16 encoding is invalid.");
					}

					return std::make_pair(
						res,
						begin
					);
				}
			}
			else if (!Internal::IsUtf16SurrogateSecond(first))
			// !Surrogate First && !Surrogate Second
			{
				char32_t res = static_cast<char32_t>(first & 0xFFFF);;

				if (!Internal::IsValidCodePt(res))
				{
					throw UtfConversionException("Invalid Code Point" " - "
						"The code point read from the given UTF-16 encoding is invalid.");
				}

				return std::make_pair(
					res,
					begin
				);
			}

			throw UtfConversionException("Invalid Encoding" " - "
				"Invalid UTF-16 leading bytes.");
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline std::pair<char32_t, InputIt> Utf32ToCodePtOnce(InputIt begin, InputIt end)
		{
			using ValueType = typename std::iterator_traits<InputIt>::value_type;

			if (begin == end)
			{
				throw UtfConversionException("Unexpected Ending" " - "
					"String ends unexpected while reading the next UTF-32 bytes.");
			}
			if (*begin & Internal::TrailingZeros<ValueType, 4>())
			{
				throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-32 bytes.");
			}

			char32_t res = *begin & 0xFFFFFFFF;
			++begin;

			if(!Internal::IsValidCodePt(res))
			{
				throw UtfConversionException("Invalid Encoding" " - "
					"Invalid UTF-32 bytes.");
			}

			return std::make_pair(
				res,
				begin
			);
		}

		template<typename OutputIt>
		inline void CodePtToUtf16Once(char32_t val, OutputIt oit)
		{
			if (!Internal::IsValidCodePt(val))
			{
				throw UtfConversionException("Invalid UTF Code Point" " - "
					+ std::to_string(val) + " is not a valid UTF code point.");
			}

			char16_t resUtf[2] = { 0 };

			if ((0x0000 <= val && val <= 0xD7FF) ||
				(0xE000 <= val && val <= 0xFFFF))
			// Single 16 bits encoding
			{
				resUtf[0] = static_cast<char16_t>(val);
				std::copy(std::begin(resUtf), std::begin(resUtf) + 1, oit);
			}
			else
			// Surrogate Pairs
			{
				char32_t code = (val - 0x10000);

				resUtf[0] = 0xD800 | (code >> 10);
				resUtf[1] = 0xDC00 | (code & 0x3FF);

				std::copy(std::begin(resUtf), std::end(resUtf), oit);
			}
		}

		inline size_t CodePtToUtf16OnceGetSize(char32_t val)
		{
			if (!Internal::IsValidCodePt(val))
			{
				throw UtfConversionException("Invalid UTF Code Point" " - "
					+ std::to_string(val) + " is not a valid UTF code point.");
			}

			if ((0x0000 <= val && val <= 0xD7FF) ||
				(0xE000 <= val && val <= 0xFFFF))
			// Single 16 bits encoding
			{
				return 1;
			}
			else
			// Surrogate Pairs
			{
				return 2;
			}
		}

		template<typename OutputIt>
		inline void CodePtToUtf32Once(char32_t val, OutputIt oit)
		{
			if (!Internal::IsValidCodePt(val))
			{
				throw UtfConversionException("Invalid UTF Code Point" " - "
					+ std::to_string(val) + " is not a valid UTF code point.");
			}

			char32_t resUtf[1] = { static_cast<char32_t>(val) };

			std::copy(std::begin(resUtf), std::end(resUtf), oit);
		}

		inline size_t CodePtToUtf32OnceGetSize(char32_t val)
		{
			if (!Internal::IsValidCodePt(val))
			{
				throw UtfConversionException("Invalid UTF Code Point" " - "
					+ std::to_string(val) + " is not a valid UTF code point.");
			}

			return 1;
		}

		template<typename InBoundFunc, typename OutBoundFunc, typename InputIt, typename OutputIt>
		inline InputIt UtfConvertOnce(InBoundFunc inFunc, OutBoundFunc outFunc,
			InputIt begin, InputIt end,
			OutputIt dest)
		{
			auto codePtRes = inFunc(begin, end);
			outFunc(codePtRes.first, dest);
			return codePtRes.second;
		}

		template<typename InBoundFunc, typename OutBoundFunc, typename InputIt, typename OutputIt>
		inline void UtfConvert(InBoundFunc inFunc, OutBoundFunc outFunc,
			InputIt begin, InputIt end,
			OutputIt dest)
		{
			while (begin != end)
			{
				begin = UtfConvertOnce(inFunc, outFunc, begin, end, dest);
			}
		}

		template<typename InBoundFunc, typename OutBoundFunc, typename InputIt>
		inline std::pair<size_t, InputIt> UtfConvertOnceGetSize(InBoundFunc inFunc, OutBoundFunc outFunc,
			InputIt begin, InputIt end)
		{
			auto codePtRes = inFunc(begin, end);
			size_t size = outFunc(codePtRes.first);
			return std::make_pair(
				size,
				codePtRes.second
			);
		}

		template<typename InBoundFunc, typename OutBoundFunc, typename InputIt>
		inline size_t UtfConvertGetSize(InBoundFunc inFunc, OutBoundFunc outFunc,
			InputIt begin, InputIt end)
		{
			size_t size = 0;
			while (begin != end)
			{
				size_t tmp = 0;
				std::tie(tmp, begin) = UtfConvertOnceGetSize(inFunc, outFunc, begin, end);
				size += tmp;
			}
			return size;
		}


		// ==========  UTF-8 --> UTF-16

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline void Utf8ToUtf16(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf8ToCodePtOnce<InputIt>, CodePtToUtf16Once<OutputIt>,
				begin, end, dest);
		}

		inline std::u16string Utf8ToUtf16(const std::string& utf8)
		{
			std::u16string resUtfStr;

			Utf8ToUtf16(utf8.begin(), utf8.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline InputIt Utf8ToUtf16Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf8ToCodePtOnce<InputIt>, CodePtToUtf16Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf8ToUtf16OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf8ToCodePtOnce<InputIt>, CodePtToUtf16OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline size_t Utf8ToUtf16GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf8ToCodePtOnce<InputIt>, CodePtToUtf16OnceGetSize,
				begin, end);
		}

		// ==========  UTF-8 --> UTF-32

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline void Utf8ToUtf32(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf8ToCodePtOnce<InputIt>, CodePtToUtf32Once<OutputIt>,
				begin, end, dest);
		}

		inline std::u32string Utf8ToUtf32(const std::string& utf8)
		{
			std::u32string resUtfStr;

			Utf8ToUtf32(utf8.begin(), utf8.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline InputIt Utf8ToUtf32Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf8ToCodePtOnce<InputIt>, CodePtToUtf32Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf8ToUtf32OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf8ToCodePtOnce<InputIt>, CodePtToUtf32OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint8_t), int> = 0>
		inline size_t Utf8ToUtf32GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf8ToCodePtOnce<InputIt>, CodePtToUtf32OnceGetSize,
				begin, end);
		}

		// ==========  UTF-16 --> UTF-8

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline void Utf16ToUtf8(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf16ToCodePtOnce<InputIt>, CodePtToUtf8Once<OutputIt>,
				begin, end, dest);
		}

		inline std::string Utf16ToUtf8(const std::u16string& in)
		{
			std::string resUtfStr;

			Utf16ToUtf8(in.begin(), in.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline InputIt Utf16ToUtf8Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf16ToCodePtOnce<InputIt>, CodePtToUtf8Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf16ToUtf8OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf16ToCodePtOnce<InputIt>, CodePtToUtf8OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline size_t Utf16ToUtf8GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf16ToCodePtOnce<InputIt>, CodePtToUtf8OnceGetSize,
				begin, end);
		}

		// ==========  UTF-16 --> UTF-32

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline void Utf16ToUtf32(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf16ToCodePtOnce<InputIt>, CodePtToUtf32Once<OutputIt>,
				begin, end, dest);
		}

		inline std::u32string Utf16ToUtf32(const std::u16string& in)
		{
			std::u32string resUtfStr;

			Utf16ToUtf32(in.begin(), in.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline InputIt Utf16ToUtf32Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf16ToCodePtOnce<InputIt>, CodePtToUtf32Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf16ToUtf32OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf16ToCodePtOnce<InputIt>, CodePtToUtf32OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint16_t), int> = 0>
		inline size_t Utf16ToUtf32GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf16ToCodePtOnce<InputIt>, CodePtToUtf32OnceGetSize,
				begin, end);
		}

		// ==========  UTF-32 --> UTF-8

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline void Utf32ToUtf8(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf32ToCodePtOnce<InputIt>, CodePtToUtf8Once<OutputIt>,
				begin, end, dest);
		}

		inline std::string Utf32ToUtf8(const std::u32string& in)
		{
			std::string resUtfStr;

			Utf32ToUtf8(in.begin(), in.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline InputIt Utf32ToUtf8Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf32ToCodePtOnce<InputIt>, CodePtToUtf8Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf32ToUtf8OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf32ToCodePtOnce<InputIt>, CodePtToUtf8OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline size_t Utf32ToUtf8GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf32ToCodePtOnce<InputIt>, CodePtToUtf8OnceGetSize,
				begin, end);
		}

		// ==========  UTF-32 --> UTF-16

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline void Utf32ToUtf16(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvert(Utf32ToCodePtOnce<InputIt>, CodePtToUtf16Once<OutputIt>,
				begin, end, dest);
		}

		inline std::u16string Utf32ToUtf16(const std::u32string& in)
		{
			std::u16string resUtfStr;

			Utf32ToUtf16(in.begin(), in.end(), std::back_inserter(resUtfStr));

			return resUtfStr;
		}

		template<typename InputIt, typename OutputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline InputIt Utf32ToUtf16Once(InputIt begin, InputIt end, OutputIt dest)
		{
			return UtfConvertOnce(Utf32ToCodePtOnce<InputIt>, CodePtToUtf16Once<OutputIt>,
				begin, end, dest);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline std::pair<size_t, InputIt> Utf32ToUtf16OnceGetSize(InputIt begin, InputIt end)
		{
			return UtfConvertOnceGetSize(Utf32ToCodePtOnce<InputIt>, CodePtToUtf16OnceGetSize,
				begin, end);
		}

		template<typename InputIt,
			Internal::EnableIfT<sizeof(typename std::iterator_traits<InputIt>::value_type) >= sizeof(uint32_t), int> = 0>
		inline size_t Utf32ToUtf16GetSize(InputIt begin, InputIt end)
		{
			return UtfConvertGetSize(Utf32ToCodePtOnce<InputIt>, CodePtToUtf16OnceGetSize,
				begin, end);
		}
	}
}
