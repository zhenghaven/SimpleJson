#pragma once

#include <cstddef>
#include <cstdint>

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
