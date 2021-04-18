#pragma once

#include <string>
#include <utility>
#include <stdexcept>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	/**
	 * @brief The exception base class for all SimpleJson classes' exceptions.
	 *        This class is inherited from std::runtime_error class.
	 *
	 */
	class Exception : public std::runtime_error
	{
	public:

		/**
		 * @brief Construct a new Exception object.
		 *        Same usage as the one in std::runtime_error.
		 *
		 * @param what_arg explanatory string
		 */
		Exception(const std::string& what_arg) :
			std::runtime_error(what_arg)
		{}

		/**
		 * @brief Construct a new Exception object.
		 *        Same usage as the one in std::runtime_error.
		 *
		 * @param what_arg explanatory string
		 */
		Exception(const char* what_arg) :
			std::runtime_error(what_arg)
		{}

		/**
		 * @brief Construct a new Exception object.
		 *        Same usage as the one in std::runtime_error.
		 *
		 * @param other another exception object to copy
		 */
		Exception(const Exception& other) noexcept :
			std::runtime_error(other)
		{}

		/**
		 * @brief Destroy the Exception object
		 *
		 */
		virtual ~Exception()
		{}
	};

	/**
	 * @brief This exception is thrown when a unsupported operation is called to
	 *        a JSON object. For example, calling a GetString operation to a
	 *        integer JSON object.
	 */
	class OperationUnsupportedException : public Exception
	{
	public:

		/**
		 * @brief Construct a new OperationUnsupportedException object.
		 *
		 * @param  typeName    The name of the JSON type.
		 * @param  opName      The name of the operation.
		 */
		OperationUnsupportedException(const char* typeName, const char* opName) :
			Exception(
				std::string("The type of this JSON object is ") + typeName +
				", which doesn't support operation of " + opName + "."
			)
		{}

		/**
		 * @brief Destroy the OperationUnsupportedException object
		 *
		 */
		virtual ~OperationUnsupportedException()
		{}
	};

	/**
	 * @brief This exception is thrown when error occurred during parsing.
	 */
	class ParseError : public Exception
	{
	public:

		template<typename InputIt>
		static bool IsLineEnd(InputIt it)
		{
			return (*it == '\r') ||
				(*it == '\n');
		}

		template<typename InputIt>
		static InputIt SkipLineEnd(InputIt begin, InputIt end)
		{
			InputIt it = begin;

			if (begin != end && *it == '\r')
			{
				++it;
			}
			if (begin != end && *it == '\n')
			{
				++it;
			}

			return it;
		}

		template<typename InputIt>
		static std::pair<size_t /* line */, size_t /* col */> CalcLineCol(InputIt oriPos, InputIt end)
		{
			size_t lineNum = 0;
			size_t colNum = 0;
			InputIt it = oriPos;

			while(it != end)
			{
				if (IsLineEnd(it))
				{
					it = SkipLineEnd(it, end);
					++lineNum;
					colNum = 0;
				}
				else
				{
					++colNum;
					++it;
				}
			}

			return std::make_pair(lineNum, colNum);
		}

		template<typename InputIt>
		static std::string ConErrorMsg(const std::string& issue, InputIt oriPos, InputIt end)
		{
			size_t lineNum = 0;
			size_t colNum = 0;

			std::tie(lineNum, colNum) = CalcLineCol(oriPos, end);

			return std::string("Parse error - ") + issue +
				": line " + std::to_string(lineNum + 1) + " column " + std::to_string(colNum + 1) + ".";
		}

	public:

		/**
		 * @brief Construct a new ParseError object.
		 *
		 * @tparam  InputIt    The type of the input iterator.
		 *
		 * @param  issue   The string representing the issue.
		 * @param  oriPos  The original begin position.
		 * @param  end     The position where error occurred.
		 */
		template<typename InputIt>
		ParseError(const std::string& issue, InputIt oriPos, InputIt end) :
			Exception(ConErrorMsg(issue, oriPos, end))
		{}

		/**
		 * @brief Destroy the ParseError object
		 *
		 */
		virtual ~ParseError()
		{}
	};

	/**
	 * @brief An exception class that is thrown when the argument given to the
	 *        function call is invalid.
	 *
	 */
	class InvalidArgumentException : public Exception
	{
	public:

		/**
		 * @brief Construct a new Invalid Argument Exception object
		 *
		 * @param what_arg explanatory string
		 */
		InvalidArgumentException(const char* what_arg) :
			Exception(what_arg)
		{}

		/**
		 * @brief Construct a new Invalid Argument Exception object
		 *
		 * @param what_arg explanatory string
		 */
		InvalidArgumentException(const std::string& what_arg) :
			Exception(what_arg)
		{}

		InvalidArgumentException(const InvalidArgumentException& other) noexcept :
			Exception(other)
		{}

		/**
		 * @brief Destroy the Invalid Argument Exception object
		 *
		 */
		virtual ~InvalidArgumentException()
		{}
	};
}
