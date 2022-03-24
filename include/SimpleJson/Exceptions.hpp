// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "SimpleObjects.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{

/**
 * @brief This exception is thrown when error occurred during parsing.
 */
class ParseError : public Internal::Obj::Exception
{
public:

	static std::string ConErrorMsg(
		const std::string& issue, size_t lineNum, size_t colNum)
	{
		return ("Parse error - " + issue) +
			" (line " + std::to_string(lineNum + 1) +
			" column " + std::to_string(colNum + 1) + ")";
	}

public:

	/**
	 * @brief Construct a new Parse Error exception
	 *
	 * @param issue    What was the issue?
	 * @param lineNum  Where the parse error happened in terms of line number
	 * @param colNum   Where the parse error happened in terms of colum number
	 */
	ParseError(const std::string& issue, size_t lineNum, size_t colNum) :
		Exception(ConErrorMsg(issue, lineNum, colNum)),
		m_lineNum(lineNum),
		m_colNum(colNum)
	{}

	/**
	 * @brief Construct a new Parse Error exception
	 *
	 * @param issue    What was the issue?
	 */
	explicit ParseError(const std::string& issue) :
		Exception(issue),
		m_lineNum(0),
		m_colNum(0)
	{}

	// LCOV_EXCL_START
	/**
	 * @brief Destroy the ParseError object
	 *
	 */
	virtual ~ParseError() = default;
	// LCOV_EXCL_STOP

private:

	size_t m_lineNum;
	size_t m_colNum;
}; // class ParseError

} // namespace SimpleJson
