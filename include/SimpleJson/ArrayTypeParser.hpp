#pragma once

#include <algorithm>

#include "ParserBase.hpp"

#include "ArrayType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class ArrayTypeParser : public ParserBase<InputIt>
		{
		public:

			ArrayTypeParser() = default;

			virtual ~ArrayTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override
			{
				ArrayType::_TypeBase res;

				std::tie(res, begin) = ArrayType::_TypeBase::ParsePartial(begin, end, oriPos);

				return std::make_pair(
					MakeUniquePtr::make_unique<ArrayType>(std::move(res)),
					begin
				);
			}

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override
			{
				return MakeUniquePtr::make_unique<ArrayType>(ArrayType::_TypeBase::Parse(begin, end));
			}
		};
	}
}
