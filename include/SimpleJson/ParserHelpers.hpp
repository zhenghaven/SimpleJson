#pragma once

#include <string>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Parser
	{
		template<typename InputIt>
		bool IsSpace(InputIt it)
		{
			return (*it == ' ') ||
				(*it == '\r') ||
				(*it == '\n') ||
				(*it == '\t') ||
				(*it == '\v') ||
				(*it == '\f');
		}

		template<typename InputIt>
		InputIt SkipLeadingSpace(InputIt begin, InputIt end)
		{
			InputIt res = begin;

			while(res != end && IsSpace(res))
			{
				++res;
			}

			return res;
		}
	}
}
