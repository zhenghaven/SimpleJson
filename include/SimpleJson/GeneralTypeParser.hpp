#pragma once

#include "ParserBase.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class GeneralTypeParser : public ParserBase<InputIt>
		{
		public:

			GeneralTypeParser() = default;

			virtual ~GeneralTypeParser()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParseNumericalPartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const;

			virtual bool IsNumerical(
				InputIt begin, InputIt end
			) const;

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const override;

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const override;
		};
	}
}
