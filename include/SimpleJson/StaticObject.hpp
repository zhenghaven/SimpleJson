#pragma once

#include "ConstMap.hpp"
#include "ConstSequence.hpp"

#include "Internal/ParserHelpers.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	template<typename _ConstMapType>
	class StaticObject
	{
	public: // Static members:

		using ConstMapType = _ConstMapType;

		static_assert(!ConstMapType::HasDupKey::value, "The keys in the object should not have duplicates");

	public:
		StaticObject() :
			m_map()
		{}

		virtual ~StaticObject()
		{}

		template<typename _KeyType>
		typename std::add_lvalue_reference<
			typename ConstMapType::template FindValType<_KeyType>
		>::type
		Get()
		{
			using _ValType = typename ConstMapType::template FindValType<_KeyType>;
			static_assert(!std::is_same<_ValType, ConstMapError>::value, "The given key is not found in the map");

			return m_map.template GetVal<_KeyType>();
		}

		template<typename _KeyType>
		typename std::add_lvalue_reference<
			typename std::add_const<
				typename ConstMapType::template FindValType<_KeyType>
			>::type
		>::type
		Get() const
		{
			using _ValType = typename ConstMapType::template FindValType<_KeyType>;
			static_assert(!std::is_same<_ValType, ConstMapError>::value, "The given key is not found in the map");

			return m_map.template GetVal<_KeyType>();
		}

	private:
		ConstMapType m_map;
	};
}
