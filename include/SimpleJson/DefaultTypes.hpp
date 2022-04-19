// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <string>

#include "NullParser.hpp"
#include "BoolParser.hpp"
#include "StringParser.hpp"
#include "NumericParser.hpp"
#include "ListParser.hpp"
#include "DictParser.hpp"

#include "GenericObjectParser.hpp"

#include "NullWriter.hpp"
#include "NumericWriter.hpp"
#include "StringWriter.hpp"
#include "ListWriter.hpp"
#include "DictWriter.hpp"
#include "ObjectWriter.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{

using NullParser = NullParserImpl<std::string, Internal::Obj::Null>;
using BoolParser = BoolParserImpl<std::string, Internal::Obj::Bool>;
using StringParser = StringParserImpl<std::string, Internal::Obj::String>;

using DictKeyParser = StringParserImpl<
	std::string, Internal::Obj::String, Internal::Obj::HashableObject>;

using GenericNumberParser = GenericNumberParserImpl<std::string,
	Internal::Obj::Int64,
	Internal::Obj::Double,
	Internal::Obj::Object>;

using IntegerParser = IntegerParserImpl<std::string, Internal::Obj::Int64>;
using RealNumParser = RealNumParserImpl<std::string, Internal::Obj::Double>;

template<typename _ItemParser>
using ListParserT = ListParserImpl<
	std::string,
	_ItemParser,
	Internal::Obj::ListT<typename _ItemParser::RetType> >;

template<typename _ValParser>
using DictParserT = DictParserImpl<
	std::string,
	DictKeyParser,
	_ValParser,
	Internal::Obj::DictT<
		typename DictKeyParser::RetType, typename _ValParser::RetType> >;

using GenericObjectParser = GenericObjectParserImpl<
	std::string,
	Internal::Obj::Null,
	Internal::Obj::Bool,
	Internal::Obj::Int64,
	Internal::Obj::Double,
	Internal::Obj::String,
	Internal::Obj::HashableObject,
	Internal::Obj::ListT,
	Internal::Obj::DictT,
	Internal::Obj::Object>;

// ===================== Writers =====================

using JsonWriterNull = JsonWriterNullImpl<std::string>;

using JsonWriterNumeric = JsonWriterNumericImpl<std::string>;

using JsonWriterString = JsonWriterStringImpl<char, std::string, std::string>;

template<typename _ValWriter>
using JsonWriterListT =
	JsonWriterListImpl<_ValWriter, std::string, std::string>;

template<typename _KeyWriter, typename _ValWriter>
using JsonWriterDictT =
	JsonWriterDictImpl<_KeyWriter, _ValWriter, std::string, std::string>;

using JsonWriterKey =
	JsonWriterKeyImpl<
		JsonWriterNull,
		JsonWriterNumeric,
		JsonWriterString,
		std::string,
		std::string>;

using JsonWriterObject =
	JsonWriterObjectImpl<
		JsonWriterNull,
		JsonWriterNumeric,
		JsonWriterString,
		JsonWriterKey,
		std::string,
		std::string>;

} // namespace SimpleJson
