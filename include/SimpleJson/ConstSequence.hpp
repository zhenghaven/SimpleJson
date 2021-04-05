#pragma once

#include <cstdint>

#include <array>
#include <string>
#include <type_traits>

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace ConstSequenceInternal
	{
		template<size_t Len, typename T, T headArg, T... args>
		struct StripTailImpl;

		template<size_t Len, char headArg, char... args>
		struct StripStrTailImpl;
	}

	template<typename _DataType, _DataType... data>
	struct DataSequence
	{
		using DataType = _DataType;

		template <DataType pre>
		using Prepend = DataSequence<DataType, pre, data...>;

		template<size_t Len>
		using StripTail = typename ConstSequenceInternal::StripTailImpl<Len, DataType, data...>::type;

		static constexpr size_t sk_size = sizeof...(data);
		static constexpr DataType const sk_cdata[sk_size]{ data... };
		static constexpr std::array<DataType, sk_size> sk_data{ data... };

		static const DataType* GetCData()
		{
			static DataType lcData[sk_size] = { data... };

			return lcData;
		}

		static const std::array<DataType, sk_size>& GetData()
		{
			static std::array<DataType, sk_size> lcData{ data... };

			return lcData;
		}
	};

	template<char... chars>
	struct StrSequence : public DataSequence<char, chars...>
	{
		using _Base = DataSequence<char, chars...>;

		template<char pre>
		using StrPrepend = StrSequence<pre, chars...>;

		template<size_t Len>
		using StripStrTail = typename ConstSequenceInternal::StripStrTailImpl<Len, chars...>::type;

		static const char* GetCStr()
		{
			static char lcStr[_Base::sk_size + 1] = { chars..., '\0' };

			return lcStr;
		}

		static const std::string& GetStr()
		{
			static const std::array<char, _Base::sk_size>& lcData = _Base::GetData();
			static const std::string lcStr(lcData.begin(), lcData.end());

			return lcStr;
		}
	};

	namespace ConstSequenceInternal
	{
		template<typename T, T headArg, T... args>
		struct StripTailImpl<1, T, headArg, args...>
		{
			using type = DataSequence<T, headArg>;
		};

		template<size_t Len, typename T, T headArg, T... args>
		struct StripTailImpl
		{
			using _type = typename StripTailImpl<Len - 1, T, args...>::type;

			using type = typename _type::template Prepend<headArg>;
		};

		template<char headArg, char... args>
		struct StripStrTailImpl<1, headArg, args...>
		{
			using type = StrSequence<headArg>;
		};

		template<size_t Len, char headArg, char... args>
		struct StripStrTailImpl
		{
			using _type = typename StripStrTailImpl<Len - 1, args...>::type;

			using type = typename _type::template StrPrepend<headArg>;
		};
	}

	template<size_t arrayLen, typename _DataType, _DataType... data>
	struct GetSequence;

	template<size_t arrayLen, typename _DataType, _DataType... data>
	struct GetSequence
	{
		using type = typename DataSequence<_DataType, data...>::template StripTail<arrayLen>;
	};

	template<size_t arrayLen, char... data>
	struct GetSequence<arrayLen, char, data...>
	{
		using type = typename StrSequence<data...>::template StripStrTail<arrayLen - 1>;
	};

	//template<typename StrStruct, size_t len, char... chars>
	//struct ExpandStrImpl
	//{
	//	using Expanded = typename ExpandStrImpl<StrStruct, len - 1, StrStruct::sk_str[len - 1], chars... >::Expanded;
	//};
	//
	//template<typename StrStruct, char... chars>
	//struct ExpandStrImpl<StrStruct, 0, chars...>
	//{
	//	using Expanded = StrSequence<chars...>;
	//};
	//
	//template<typename StrStruct>
	//using ExpandStr = typename ExpandStrImpl<StrStruct, sizeof(StrStruct::sk_str) - 1>::Expanded;

	namespace ConstSequenceInternal
	{
		template<size_t i, typename T, size_t n>
		constexpr T ExpandArrayImpl(const T(&a)[n])
		{
			return i < n ? a[i] : T();
		}
	}

}

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
#define SIMPLEJSON_EXPANDARRAY_2(  S, I) SimpleJson::ConstSequenceInternal::ExpandArrayImpl<(I)>(S),SimpleJson::ConstSequenceInternal::ExpandArrayImpl<(I) + 1>(S)
#else
#define SIMPLEJSON_EXPANDARRAY_2(  S, I) SIMPLEJSON_CUSTOMIZED_NAMESPACE::ConstSequenceInternal::ExpandArrayImpl<(I)>(S),SIMPLEJSON_CUSTOMIZED_NAMESPACE::ConstSequenceInternal::ExpandArrayImpl<(I) + 1>(S)
#endif

#define SIMPLEJSON_EXPANDARRAY_4(  S, I) SIMPLEJSON_EXPANDARRAY_2(  S, (I)),SIMPLEJSON_EXPANDARRAY_2(  S, (I) + 2)
#define SIMPLEJSON_EXPANDARRAY_8(  S, I) SIMPLEJSON_EXPANDARRAY_4(  S, (I)),SIMPLEJSON_EXPANDARRAY_4(  S, (I) + 4)
#define SIMPLEJSON_EXPANDARRAY_16( S, I) SIMPLEJSON_EXPANDARRAY_8(  S, (I)),SIMPLEJSON_EXPANDARRAY_8(  S, (I) + 8)
#define SIMPLEJSON_EXPANDARRAY_32( S, I) SIMPLEJSON_EXPANDARRAY_16( S, (I)),SIMPLEJSON_EXPANDARRAY_16( S, (I) + 16)
#define SIMPLEJSON_EXPANDARRAY_64( S, I) SIMPLEJSON_EXPANDARRAY_32( S, (I)),SIMPLEJSON_EXPANDARRAY_32( S, (I) + 32)
#define SIMPLEJSON_EXPANDARRAY_128(S, I) SIMPLEJSON_EXPANDARRAY_64( S, (I)),SIMPLEJSON_EXPANDARRAY_64( S, (I) + 64)
#define SIMPLEJSON_EXPANDARRAY_256(S, I) SIMPLEJSON_EXPANDARRAY_128(S, (I)),SIMPLEJSON_EXPANDARRAY_128(S, (I) + 128)
#define SIMPLEJSON_EXPANDARRAY(S) SIMPLEJSON_EXPANDARRAY_256(S, 0)

#define SIMPLEJSON_CARRAY_DATATYPE(S) std::decay<decltype(S[0])>::type
#define SIMPLEJSON_CARRAY_DATASIZE(S) (sizeof(SIMPLEJSON_CARRAY_DATATYPE(S)))
#define SIMPLEJSON_CARRAY_LEN(S)      (sizeof(S) / SIMPLEJSON_CARRAY_DATASIZE(S))

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
#define SIMPLEJSON_EXPANDARRAY_2(  S, I) SimpleJson::ConstSequenceInternal::ExpandArrayImpl<(I)>(S),SimpleJson::ConstSequenceInternal::ExpandArrayImpl<(I) + 1>(S)
#define SIMPLEJSON_CONST_ARRAY(S)  SimpleJson::GetSequence<SIMPLEJSON_CARRAY_LEN(S), SIMPLEJSON_CARRAY_DATATYPE(S), SIMPLEJSON_EXPANDARRAY(S)>::type
#define SIMPLEJSON_CONST_STRING(S) SimpleJson::GetSequence<SIMPLEJSON_CARRAY_LEN(S), SIMPLEJSON_CARRAY_DATATYPE(S), SIMPLEJSON_EXPANDARRAY(S)>::type
#else
#define SIMPLEJSON_EXPANDARRAY_2(  S, I) SIMPLEJSON_CUSTOMIZED_NAMESPACE::ConstSequenceInternal::ExpandArrayImpl<(I)>(S),SIMPLEJSON_CUSTOMIZED_NAMESPACE::ConstSequenceInternal::ExpandArrayImpl<(I) + 1>(S)
#define SIMPLEJSON_CONST_ARRAY(S)  SIMPLEJSON_CUSTOMIZED_NAMESPACE::GetSequence<SIMPLEJSON_CARRAY_LEN(S), SIMPLEJSON_CARRAY_DATATYPE(S), SIMPLEJSON_EXPANDARRAY(S)>::type
#define SIMPLEJSON_CONST_STRING(S) SIMPLEJSON_CUSTOMIZED_NAMESPACE::GetSequence<SIMPLEJSON_CARRAY_LEN(S), SIMPLEJSON_CARRAY_DATATYPE(S), SIMPLEJSON_EXPANDARRAY(S)>::type
#endif
