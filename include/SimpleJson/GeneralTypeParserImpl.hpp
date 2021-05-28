#pragma once

#include "GeneralTypeParser.hpp"

#include "NullTypeParser.hpp"
#include "BoolTypeParser.hpp"
#include "IntegerTypeParser.hpp"
#include "RealTypeParser.hpp"
#include "ArrayTypeParser.hpp"
#include "StringTypeParser.hpp"

#include "Internal/NumberParser.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{

		template<typename InputIt>
		inline std::pair<std::unique_ptr<Json>, InputIt> GeneralTypeParser<InputIt>::ParseNumericalPartial(
			InputIt begin, InputIt end, const InputIt oriPos
		) const
		{
			std::string intStr;
			std::string fracStr;
			std::string expStr;

			Internal::ParseNumberSignInplace(begin, end, oriPos,
				std::back_inserter(intStr), std::back_inserter(fracStr),
				std::back_inserter(expStr));

			if (fracStr.size() > 0 || expStr.size() > 0)
			{
				// Real Number
				return std::make_pair(
					MakeUniquePtr::make_unique<RealType>(
						Real<>::FromComponents(intStr, fracStr, expStr)
					),
					begin
				);
			}
			else
			{
				// Integer
				return std::make_pair(
					MakeUniquePtr::make_unique<IntType>(
						Integer<>::FromComponents(intStr, fracStr, expStr)
					),
					begin
				);
			}
		}

		template<typename InputIt>
		inline std::pair<std::unique_ptr<Json>, InputIt> GeneralTypeParser<InputIt>::ParsePartial(
			InputIt begin, InputIt end, const InputIt oriPos
		) const
		{
			begin = Internal::SkipLeadingSpace(begin, end);

			switch(*begin)
			{
				case 'n':   // null   -- Null
					return NullTypeParser<InputIt>().ParsePartial(begin, end, oriPos);
				case 't':   // true   -- Bool
				case 'f':   // false  -- Bool
					return BoolTypeParser<InputIt>().ParsePartial(begin, end, oriPos);
				case '[':   // []     -- Array
					return ArrayTypeParser<InputIt>().ParsePartial(begin, end, oriPos);
				case '{':   // {}     -- Object
				case '\"':  // "..."  -- String
					return StringTypeParser<InputIt>().ParsePartial(begin, end, oriPos);
				case '-':
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
					return ParseNumericalPartial(begin, end, oriPos);
				default:
					break;
			}

			throw ParseError("Unexpected string", oriPos, begin);
		}

		template<typename InputIt>
		inline std::unique_ptr<Json> GeneralTypeParser<InputIt>::Parse(
			InputIt begin, InputIt end
		) const
		{
			std::unique_ptr<Json> res;
			InputIt nextBegin = begin;

			std::tie(res, nextBegin) = ParsePartial(begin, end, begin);

			if(nextBegin != end)
			{
				throw ParseError("Extra Data", begin, nextBegin);
			}

			return res;
		}
	}

	template<typename InputIt>
	inline std::unique_ptr<Json> Json::Parse(InputIt begin, InputIt end)
	{
		return Parser::GeneralTypeParser<InputIt>().Parse(begin, end);
	}
}
