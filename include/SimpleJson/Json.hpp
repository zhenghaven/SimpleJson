#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <iterator>

#include "Config.hpp"
#include "Exceptions.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	enum class DataTypes
	{
		Null,
		String,
		Boolean,
		Array,
		Integer,
		Real,
	};

	class ArrayType;
	class ObjType;
	class StringType;

	class Json
	{
	public: // Static member:

		template<typename InputIt>
		static std::unique_ptr<Json> Parse(InputIt begin, InputIt end);

	public:
		Json() = default;

		// Json()
		// {}

		// Json(const Json& other)
		// {}

		// Json(Json&& other)
		// {}

		virtual ~Json()
		{}

		// Json& operator=(const Json& rhs)
		// {
		// 	return *this;
		// }

		// Json& operator=(Json&& rhs)
		// {
		// 	return *this;
		// }

		virtual StringType& GetString()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetString");
		}

		virtual const StringType& GetString() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetString");
		}

		virtual std::string& GetUtf8String()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUtf8String");
		}

		virtual const std::string& GetUtf8String() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUtf8String");
		}

		virtual const DefaultIntType& GetInt() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt");
		}

		virtual DefaultIntType& GetInt()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt");
		}

		template<typename _TargetType>
		_TargetType GetIntCasted() const
		{
			const DefaultIntType& tmp = GetInt();

			if (std::numeric_limits<_TargetType>::lowest() <= tmp &&
				tmp <= std::numeric_limits<_TargetType>::max())
			{
				return _TargetType(tmp);
			}
			else
			{
				throw RangeErrorException("The value stored is out side of the range of the given target type.");
			}
		}

		virtual const DefaultRealType& GetReal() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetReal");
		}

		virtual DefaultRealType& GetReal()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetReal");
		}

		template<typename _TargetType>
		_TargetType GetRealCasted() const
		{
			const DefaultRealType& tmp = GetReal();

			if (std::numeric_limits<_TargetType>::lowest() > tmp ||
				tmp > std::numeric_limits<_TargetType>::max() ||
				(tmp > 0 && tmp < std::numeric_limits<_TargetType>::min()) ||
				(tmp < 0 && tmp > -std::numeric_limits<_TargetType>::min()))
			{
				throw RangeErrorException("The value stored is out side of the range of the given target type.");
			}

			return _TargetType(tmp);
		}

		virtual bool GetBool() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetBool");
		}

		virtual const ArrayType& GetArray() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetArray");
		}

		virtual ArrayType& GetArray()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetArray");
		}

		virtual bool operator==(const Json& rhs) const = 0;

		virtual bool operator!=(const Json& rhs) const
		{
			return !((*this) == rhs);
		}

		virtual bool IsNull() const = 0;

		virtual DataTypes GetDataType() const noexcept = 0;

		virtual void ToString(std::back_insert_iterator<std::string> dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = gk_defaultSortKeys, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const = 0;

		virtual std::string ToString(
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			std::string res;
			ToString(std::back_inserter(res),
				indent, lineEnd,
				sortKeys, precision,
				nestLevel, addComma);
			return res;
		}

	protected:

		virtual const char* GetTypeName() const noexcept = 0;

	};
}
