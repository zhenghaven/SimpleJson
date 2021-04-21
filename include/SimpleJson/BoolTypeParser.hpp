#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "BoolType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class BoolTypeParser : public ParserBase<InputIt>
		{
		public:

			BoolTypeParser() = default;

			virtual ~BoolTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				Bool res;
				InputIt nextBegin = begin;

				std::tie(res, nextBegin) = Bool::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					MakeUniquePtr::make_unique<BoolType>(std::move(res)),
					nextBegin
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return MakeUniquePtr::make_unique<BoolType>(Bool::Parse(begin, end));
			}
		};
	}
}
