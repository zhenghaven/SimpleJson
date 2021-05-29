#pragma once

#include <cstddef>
#include <cstdint>

#include <stdexcept>
#include <type_traits>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	// Const Pair
	template<typename _KeyType, typename _ValType>
	struct ConstPair
	{
		using KeyType = _KeyType;
		using ValType = _ValType;
	};

	// Is Key Match
	template<typename _Pair1, typename _Pair2>
	struct IsConstKeyMatch : std::false_type {};

	template<typename _KeyType, typename _ValType1, typename _ValType2>
	struct IsConstKeyMatch<
		ConstPair<_KeyType, _ValType1>,
		ConstPair<_KeyType, _ValType2>
	> : std::true_type {};

	static_assert(IsConstKeyMatch<ConstPair<uint8_t, size_t>, ConstPair<uint8_t, size_t> >::value, "Programming Error.");
	static_assert(!IsConstKeyMatch<ConstPair<uint8_t, size_t>, ConstPair<uint16_t, size_t> >::value, "Programming Error.");
	static_assert(!IsConstKeyMatch<size_t, size_t>::value, "Programming Error.");

	// Is Given Key
	template<typename _Pair, typename _KeyType>
	struct IsGivenConstKey : std::false_type {};

	template<typename _KeyType, typename _ValType>
	struct IsGivenConstKey<
		ConstPair<_KeyType, _ValType>,
		_KeyType
	> : std::true_type {};

	static_assert(IsGivenConstKey<ConstPair<uint8_t, size_t>, uint8_t>::value, "Programming Error.");
	static_assert(!IsGivenConstKey<ConstPair<uint8_t, size_t>, uint16_t>::value, "Programming Error.");
	static_assert(!IsGivenConstKey<size_t, size_t>::value, "Programming Error.");

	// Const Map
	struct ConstMapError {};

	namespace Internal
	{
		template<
			typename _OutValType,
			typename _SelfKeyType,
			typename _SelfValType,
			typename _NextPairType,
			typename _GivenKeyType
		>
		struct MemberGetterHelper
		{
			static
			typename std::add_lvalue_reference<_OutValType>::type
			Get(
				typename std::add_lvalue_reference<_SelfValType>::type selfVal,
				typename std::add_lvalue_reference<_NextPairType>::type nextPair
			)
			{
				return nextPair.template GetVal<_GivenKeyType>();
			}

			static
			typename std::add_lvalue_reference<
				typename std::add_const<_OutValType>::type>::type
			Get(
				typename std::add_lvalue_reference<
					typename std::add_const<_SelfValType>::type>::type selfVal,
				typename std::add_lvalue_reference<
					typename std::add_const<_NextPairType>::type>::type nextPair
			)
			{
				return nextPair.template GetVal<_GivenKeyType>();
			}
		};

		template<
			typename _OutValType,
			typename _SelfKeyType,
			typename _SelfValType,
			typename _NextPairType
		>
		struct MemberGetterHelper<
			_OutValType,
			_SelfKeyType,
			_SelfValType,
			_NextPairType,
			_SelfKeyType
		>
		{
			static
			typename std::add_lvalue_reference<_OutValType>::type
			Get(
				typename std::add_lvalue_reference<_SelfValType>::type selfVal,
				typename std::add_lvalue_reference<_NextPairType>::type nextPair
			)
			{
				return selfVal;
			}

			static
			typename std::add_lvalue_reference<
				typename std::add_const<_OutValType>::type>::type
			Get(
				typename std::add_lvalue_reference<
					typename std::add_const<_SelfValType>::type>::type selfVal,
				typename std::add_lvalue_reference<
					typename std::add_const<_NextPairType>::type>::type nextPair
			)
			{
				return selfVal;
			}
		};

	}

	template<typename... _Pairs>
	struct ConstMap : ConstMapError {};

	template<typename _KeyType, typename _ValType>
	struct ConstMap<ConstPair<_KeyType, _ValType> >
	{
		static constexpr size_t sk_size = 1;

		template<typename _OtherKeyType>
		using FindValType = typename std::conditional<
			std::is_same<_KeyType, _OtherKeyType>::value,
			_ValType,
			ConstMapError
		>::type;

		template<typename _OtherKeyType>
		using HasKey = std::is_same<_KeyType, _OtherKeyType>;

		using HasDupKey = std::false_type;

		template<typename _OtherKeyType>
		typename std::add_lvalue_reference<
			FindValType<_OtherKeyType>
		>::type
		GetVal()
		{
			return Internal::MemberGetterHelper<
				FindValType<_OtherKeyType>,
				_KeyType,
				_ValType,
				ConstMapError,
				_OtherKeyType
			>::Get(m_val, m_null);
		}

		template<typename _OtherKeyType>
		typename std::add_lvalue_reference<
			typename std::add_const<
				FindValType<_OtherKeyType>
			>::type
		>::type
		GetVal() const
		{
			return Internal::MemberGetterHelper<
				FindValType<_OtherKeyType>,
				_KeyType,
				_ValType,
				ConstMapError,
				_OtherKeyType
			>::Get(m_val, m_null);
		}

		_ValType m_val;
		ConstMapError m_null;
	};

	template<typename _KeyType, typename _ValType, typename... _Pairs>
	struct ConstMap<ConstPair<_KeyType, _ValType>, ConstMap<_Pairs...> >
	{
		using Next = ConstMap<_Pairs...>;

		static constexpr size_t sk_size = 1 + Next::sk_size;

		template<typename _OtherKeyType>
		using FindValType = typename std::conditional<
			std::is_same<_KeyType, _OtherKeyType>::value,
			_ValType,
			typename Next::template FindValType<_OtherKeyType>
		>::type;

		template<typename _OtherKeyType>
		using HasKey = typename std::conditional<
			std::is_same<_KeyType, _OtherKeyType>::value,
			std::true_type,
			typename Next::template HasKey<_OtherKeyType>
		>::type;

		using HasDupKey = typename std::conditional<
			Next::template HasKey<_KeyType>::value || Next::HasDupKey::value,
			std::true_type,
			std::false_type
		>::type;

		template<typename _OtherKeyType>
		typename std::add_lvalue_reference<
			FindValType<_OtherKeyType>
		>::type
		GetVal()
		{
			return Internal::MemberGetterHelper<
				FindValType<_OtherKeyType>,
				_KeyType,
				_ValType,
				Next,
				_OtherKeyType
			>::Get(m_val, m_next);
		}

		template<typename _OtherKeyType>
		typename std::add_lvalue_reference<
			typename std::add_const<
				FindValType<_OtherKeyType>
			>::type
		>::type
		GetVal() const
		{
			return Internal::MemberGetterHelper<
				FindValType<_OtherKeyType>,
				_KeyType,
				_ValType,
				Next,
				_OtherKeyType
			>::Get(m_val, m_next);
		}

		_ValType m_val;
		Next m_next;
	};

	// Build Const Map
	template<typename... _Pairs>
	struct BuildConstMap : std::false_type {};

	template<typename _KeyType, typename _ValType>
	struct BuildConstMap<ConstPair<_KeyType, _ValType> >
	{
		using type = ConstMap<ConstPair<_KeyType, _ValType> >;
	};

	template<typename _KeyType, typename _ValType, typename... _Pairs>
	struct BuildConstMap<ConstPair<_KeyType, _ValType>, _Pairs... >
	{
		using type = ConstMap<ConstPair<_KeyType, _ValType>, typename BuildConstMap<_Pairs...>::type >;
	};
}
