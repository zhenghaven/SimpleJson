#pragma once

#include "GeneralTypeParser.hpp"

#include "NullTypeParser.hpp"
#include "BoolTypeParser.hpp"
#include "ArrayTypeParser.hpp"
#include "StringTypeParser.hpp"

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
			throw ParseError("Unexpected string", oriPos, begin);
		}

		template<typename InputIt>
		inline bool GeneralTypeParser<InputIt>::IsNumerical(
			InputIt begin, InputIt end
		) const
		{
			return false;
		}

		template<typename InputIt>
		inline std::pair<std::unique_ptr<Json>, InputIt> GeneralTypeParser<InputIt>::ParsePartial(
			InputIt begin, InputIt end, const InputIt oriPos
		) const
		{
			begin = SkipLeadingSpace(begin, end);

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
				default:
					break;
			}

			if (IsNumerical(begin, end))
			{
				return ParseNumericalPartial(begin, end, oriPos);
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
