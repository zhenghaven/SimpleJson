#pragma once

#include <iterator>

#include "Exceptions.hpp"

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
			while(begin != end && IsSpace(begin))
			{
				++begin;
			}

			return begin;
		}

		template<typename InputIt>
		typename std::iterator_traits<InputIt>::value_type
		NextChar(InputIt& begin, InputIt end, const InputIt oriPos)
		{
			begin = SkipLeadingSpace(begin, end);
			if (begin != end)
			{
				return *(begin++);
			}
			throw ParseError("Unexpected Ends", oriPos, begin);
		}

		template<typename InputIt>
		typename std::iterator_traits<InputIt>::value_type
		PeekChar(InputIt& begin, InputIt end, const InputIt oriPos)
		{
			begin = SkipLeadingSpace(begin, end);
			if (begin != end)
			{
				return *begin;
			}
			throw ParseError("Unexpected Ends", oriPos, begin);
		}

		template<typename InputIt>
		typename std::iterator_traits<InputIt>::value_type
		ImmdNextChar(InputIt& begin, InputIt end, const InputIt oriPos)
		{
			if (begin != end)
			{
				return *(begin++);
			}
			throw ParseError("Unexpected Ends", oriPos, begin);
		}

		template<typename InputIt>
		typename std::iterator_traits<InputIt>::value_type
		ImmdPeekChar(InputIt& begin, InputIt end, const InputIt oriPos)
		{
			if (begin != end)
			{
				return *begin;
			}
			throw ParseError("Unexpected Ends", oriPos, begin);
		}
	}
}
