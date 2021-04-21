#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "StringType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class StringTypeParser : public ParserBase<InputIt>
		{
		public:

			StringTypeParser() = default;

			virtual ~StringTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				auto parseRes = String<>::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					MakeUniquePtr::make_unique<StringType>(std::move(parseRes.first)),
					parseRes.second
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return MakeUniquePtr::make_unique<StringType>(String<>::Parse(begin, end));
			}
		};
	}
}
