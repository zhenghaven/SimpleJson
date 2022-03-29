// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "NullParser.hpp"
#include "BoolParser.hpp"
#include "StringParser.hpp"
#include "NumericParser.hpp"
#include "ListParser.hpp"
#include "DictParser.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{


/**
 * @brief Parser for generic object
 *
 * @tparam _ContainerType Type of containers which *may* be needed during
 *                        intermediate steps.
 *                        Meanwhile, the input character type is inferred by
 *                        `_ContainerType::value_type`
 * @tparam _IntType       The type used to construct the integer type
 * @tparam _RealType      The type used to construct the real type
 * @tparam _RetType       The type that will be returned by the parser
 */
template<
	typename _ContainerType,
	typename _NullType,
	typename _BoolType,
	typename _IntType,
	typename _RealType,
	typename _StrType,
	typename _HashObjType,
	template<typename> class _ListType,
	template<typename,typename> class _DictType,
	typename _RetType>
class GenericObjectParserImpl : public ParserBase<_ContainerType, _RetType>
{
public: // static members:

	using Base = ParserBase<_ContainerType, _RetType>;
	using Self = GenericObjectParserImpl<
		_ContainerType,
		_NullType,
		_BoolType,
		_IntType,
		_RealType,
		_StrType,
		_HashObjType,
		_ListType,
		_DictType,
		_RetType>;

	using ContainerType = _ContainerType;
	using InputChType   = typename ContainerType::value_type;

	using RetType       = _RetType;
	using IteratorType  = Internal::Obj::FrIterator<InputChType, true>;
	using ISMType       = ForwardIteratorStateMachine<IteratorType>;

	using NullParser = NullParserImpl<_ContainerType, _NullType>;
	using BoolParser = BoolParserImpl<_ContainerType, _BoolType>;

	using GenericNumberParser =
		GenericNumberParserImpl<_ContainerType, _IntType, _RealType, RetType>;

	using StringParser =
		StringParserImpl<_ContainerType, _StrType>;

	using DictKeyParser =
		StringParserImpl<_ContainerType, _StrType, _HashObjType>;

	using ListParser =
		ListParserImpl<_ContainerType, Self, _ListType<RetType> >;

	using DictParser = DictParserImpl<
		_ContainerType,
		DictKeyParser,
		Self,
		_DictType<_HashObjType, RetType> >;

public:

	GenericObjectParserImpl() = default;

	// LCOV_EXCL_START
	virtual ~GenericObjectParserImpl() = default;
	// LCOV_EXCL_STOP

	using Base::Parse;

	virtual RetType Parse(InputStateMachineIf<InputChType>& ism) const override
	{
		auto ch = ism.SkipSpaceAndGetChar();

		switch(ch)
		{
			case 'n':   // null   -- Null
				return NullParser().Parse(ism);
			case 't':   // true   -- Bool
			case 'f':   // false  -- Bool
				return BoolParser().Parse(ism);
			case '-':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return GenericNumberParser().Parse(ism);
			case '\"':  // "..."  -- String
				return StringParser().Parse(ism);
			case '[':   // []     -- List
				return ListParser().Parse(ism);
			case '{':   // {}     -- Dict
				return DictParser().Parse(ism);
			default:
				break;
		}

		throw ParseError("Unexpected character",
			ism.GetLineCount(), ism.GetColCount());
	}

}; // class GenericObjectParserImpl

} // namespace SimpleJson
