#pragma once

#include <cstdint>

#include <memory>
#include <string>

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
	};

	class ArrayType;
	class ObjType;

	class Json
	{
	public: // Static member:

		template<typename InputIt>
		static std::unique_ptr<Json> Parse(InputIt begin, InputIt end);

	public:
		Json() = default;

		virtual ~Json()
		{}

		virtual std::string& GetString()
		{
			throw OperationUnsupportedException(GetTypeName(), "GetString");
		}

		virtual const std::string& GetString() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetString");
		}

		virtual int8_t GetInt8() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt8");
		}

		virtual uint8_t GetUInt8() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUInt8");
		}

		virtual int16_t GetInt16() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt16");
		}

		virtual uint16_t GetUInt16() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUInt16");
		}

		virtual int32_t GetInt32() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt32");
		}

		virtual uint32_t GetUInt32() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUInt32");
		}

		virtual int64_t GetInt64() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetInt64");
		}

		virtual uint64_t GetUInt64() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetUInt64");
		}

		virtual double GetReal() const
		{
			throw OperationUnsupportedException(GetTypeName(), "GetReal");
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

		virtual std::string ToString(const std::string& indent = "", const std::string& lineEnd = "\n", bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const = 0;

	protected:

		virtual const char* GetTypeName() const noexcept = 0;

	};
}
