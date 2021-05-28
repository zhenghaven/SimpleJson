#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "RealType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class RealTypeParser : public ParserBase<InputIt>
		{
		public:

			RealTypeParser() = default;

			virtual ~RealTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				Real<> res;
				InputIt nextBegin = begin;

				std::tie(res, nextBegin) = Real<>::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					MakeUniquePtr::make_unique<RealType>(std::move(res)),
					nextBegin
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return MakeUniquePtr::make_unique<RealType>(Real<>::Parse(begin, end));
			}
		};
	}
}
