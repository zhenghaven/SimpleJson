#pragma once

#include <tuple>

#include <string>

#include "ParserHelpers.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		namespace Internal
		{
			template<bool _IsImmd, bool _IsOptional = true, typename InputIt>
			inline bool ParseNumNegativeSign(InputIt& begin, InputIt end, const InputIt oriPos)
			{
				if (_IsOptional && (begin == end))
				{
					return false;
				}

				auto tmpCh = _IsImmd ? ImmdPeekChar(begin, end, oriPos) : PeekChar(begin, end, oriPos);
				switch (tmpCh)
				{
				case '-':
					++begin;
					return true;

				default:
					return _IsOptional ? false : throw ParseError("Expecting a negative sign", oriPos, begin);
				}
			}

			template<bool _IsImmd, bool _IsOptional = true, typename InputIt>
			inline bool ParseNumSign(InputIt& begin, InputIt end, const InputIt oriPos)
			{
				if (_IsOptional && (begin == end))
				{
					return true;
				}

				auto tmpCh = _IsImmd ? ImmdPeekChar(begin, end, oriPos) : PeekChar(begin, end, oriPos);
				switch (tmpCh)
				{
				case '-':
					++begin;
					return false;
				case '+':
					++begin;
					return true;

				default:
					return _IsOptional ? true : throw ParseError("Expecting a numeric sign", oriPos, begin);
				}
			}

			template<bool _IsOptional = true, typename InputIt, typename OutputIt>
			inline void ParseNumDigits(InputIt& begin, InputIt end, const InputIt oriPos, OutputIt dest)
			{
				size_t len = 0;

				while(true)
				{
					if (begin == end)
					{
						if (!_IsOptional && len == 0)
						{
							throw ParseError("Unexpected Ends", oriPos, begin);
						}
						else
						{
							return;
						}
					}

					auto tmpCh = ImmdPeekChar(begin, end, oriPos);

					switch (tmpCh)
					{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						std::copy(begin, begin + 1, dest);
						++begin;
						++len;
						break;

					default:
					{
						if (!_IsOptional && len == 0)
						{
							throw ParseError("Expecting a numeric value", oriPos, begin);
						}
						else
						{
							return;
						}
					}
					}
				}
			}

			template<typename InputIt, typename OutputIt>
			inline void ParseNumInt(InputIt& begin, InputIt end, const InputIt oriPos, OutputIt dest)
			{
				switch(ImmdPeekChar(begin, end, oriPos))
				{
				case '0':
					std::copy(begin, begin + 1, dest);
					++begin;
					return;

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					std::copy(begin, begin + 1, dest);
					++begin;
					return ParseNumDigits<>(begin, end, oriPos, dest);

				default:
					throw ParseError("Expecting a numeric value", oriPos, begin);
				}
			}

			template<bool _IsOptional, typename InputIt, typename OutputIt>
			inline void ParseNumFrac(InputIt& begin, InputIt end, const InputIt oriPos, OutputIt dest)
			{
				if (_IsOptional && (begin == end))
				{
					return;
				}

				if (ImmdPeekChar(begin, end, oriPos) == '.')
				{
					++begin;
					return ParseNumDigits<false>(begin, end, oriPos, dest);
				}
				else if (!_IsOptional)
				{
					throw ParseError("Expecting a decimal point", oriPos, begin);
				}
			}

			template<bool _IsOptional, typename InputIt, typename OutputIt>
			inline void ParseNumExp(InputIt& begin, InputIt end, const InputIt oriPos, bool& isPos, OutputIt dest)
			{
				if (_IsOptional && (begin == end))
				{
					return;
				}

				auto tmpCh = ImmdPeekChar(begin, end, oriPos);
				if (tmpCh == 'e' || tmpCh == 'E')
				{
					++begin;
					isPos = ParseNumSign<true, true>(begin, end, oriPos);
					return ParseNumDigits<false>(begin, end, oriPos, dest);
				}
				else if (!_IsOptional)
				{
					throw ParseError("Expecting a decimal point", oriPos, begin);
				}
			}

			template<typename InputIt, typename IntOutputIt, typename FracOutputIt, typename ExpOutputIt>
			inline void ParseNumber(InputIt& begin, InputIt end, const InputIt oriPos,
				bool& isPos, IntOutputIt idest,
				FracOutputIt fdest,
				bool& isExpPos, ExpOutputIt edest)
			{
				isPos = !ParseNumNegativeSign<false>(begin, end, oriPos);
				ParseNumInt(begin, end, oriPos, idest);
				ParseNumFrac<true>(begin, end, oriPos, fdest);
				ParseNumExp<true>(begin, end, oriPos, isExpPos, edest);
			}

		}
	}
}
