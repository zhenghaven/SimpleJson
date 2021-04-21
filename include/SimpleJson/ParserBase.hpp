#pragma once

#include <memory>
#include <utility>
#include <string>

#include "Exceptions.hpp"

#include "Internal/make_unique.hpp"
#include "Internal/ParserHelpers.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	class Json;

	namespace Parser
	{
		template<typename InputIt = std::string::const_iterator>
		class ParserBase
		{
		public:

			ParserBase() = default;

			virtual ~ParserBase()
			{}

			virtual std::pair<std::unique_ptr<Json>, InputIt> ParsePartial(
				InputIt begin, InputIt end, const InputIt oriPos
			) const = 0;

			virtual std::unique_ptr<Json> Parse(
				InputIt begin, InputIt end
			) const = 0;
		};
	}
}
