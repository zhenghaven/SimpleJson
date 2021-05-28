#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "IntegerType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class IntTypeParser : public ParserBase<InputIt>
		{
		public:

			IntTypeParser() = default;

			virtual ~IntTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				Integer<> res;
				InputIt nextBegin = begin;

				std::tie(res, nextBegin) = Integer<>::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					MakeUniquePtr::make_unique<IntType>(std::move(res)),
					nextBegin
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return MakeUniquePtr::make_unique<IntType>(Integer<>::Parse(begin, end));
			}
		};
	}
}
