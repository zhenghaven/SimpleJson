#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "NullType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class NullTypeParser : public ParserBase<InputIt>
		{
		public:

			NullTypeParser() = default;

			virtual ~NullTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				Null res;
				InputIt nextBegin = begin;

				std::tie(res, nextBegin) = Null::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					Internal::make_unique<NullType>(std::move(res)),
					nextBegin
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return Internal::make_unique<NullType>(Null::Parse(begin, end));
			}
		};
	}
}
