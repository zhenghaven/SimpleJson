#pragma once

#include "Number.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	template<typename _ValType = DefaultIntType>
	class Integer : public Number<_ValType>
	{
	public: // Static member:

		using IntValType = _ValType;
		using _Base = Number<IntValType>;

		static Integer FromComponents(const std::string& intStr,
			const std::string& fracStr,
			const std::string& expStr)
		{
			return Integer(_Base::FromComponents(intStr, fracStr, expStr));
		}

		template<typename InputIt>
		static std::pair<Integer, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			auto numRes = _Base::ParsePartial(begin, end, oriPos);

			return std::make_pair(
				Integer(std::move(numRes.first)),
				numRes.second
			);
		}

		template<typename InputIt>
		static Integer Parse(InputIt begin, InputIt end)
		{
			return Integer(_Base::Parse(begin, end));
		}

	public:
		Integer() :
			_Base::Number()
		{}

		Integer(const IntValType& val) :
			_Base::Number(val)
		{}

		Integer(IntValType&& val) :
			_Base::Number(std::forward<IntValType>(val))
		{}

		Integer(_Base&& val) noexcept:
			_Base::Number(std::forward<_Base>(val))
		{}

		Integer(const Integer& other) :
			_Base::Number(other)
		{}

		Integer(Integer&& other) noexcept:
			_Base::Number(std::forward<_Base>(other))
		{}

		virtual ~Integer()
		{}

		Integer& operator=(const Integer& rhs)
		{
			_Base::operator=(rhs);

			return *this;
		}

		Integer& operator=(Integer&& rhs)
		{
			_Base::operator=(std::forward<_Base>(rhs));

			return *this;
		}

		Integer& operator=(const IntValType& rhs)
		{
			_Base::operator=(rhs);

			return *this;
		}

		Integer& operator=(IntValType&& rhs)
		{
			_Base::operator=(std::forward<IntValType>(rhs));

			return *this;
		}

		template<typename OutputIt>
		void ToString(OutputIt dest, const
			std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			constexpr char cmaStr[] = ",";

			std::string intStr = std::to_string(_Base::GetVal());
			std::copy(intStr.begin(), intStr.end(), dest);

			if (addComma)
			{
				std::copy(std::begin(cmaStr), std::end(cmaStr) - 1, dest);
			}

			if (indent.size() > 0)
			{
				std::copy(lineEnd.begin(), lineEnd.end(), dest);
			}
		}
	};

	class IntType : public Json, public Integer<>
	{
	public: // Static members:

		using _TypeBase = Integer<>;

	public:
		IntType() :
			Json::Json(),
			_TypeBase::Integer()
		{}

		IntType(const DefaultIntType& val) :
			Json::Json(),
			_TypeBase::Integer(val)
		{}

		IntType(DefaultIntType&& val) :
			Json::Json(),
			_TypeBase::Integer(std::forward<DefaultIntType>(val))
		{}

		// IntType(const IntType& other) :
		// 	Json::Json(other),
		// 	_TypeBase::Integer(other)
		// {}

		IntType(IntType&& other) noexcept:
			Json::Json(std::forward<Json>(other)),
			_TypeBase::Integer(std::forward<_TypeBase>(other))
		{}

		IntType(_TypeBase val) :
			Json::Json(),
			_TypeBase::Integer(std::move(val))
		{}

		virtual ~IntType()
		{}

		// IntType& operator=(const IntType& rhs)
		// {
		// 	Json::operator=(rhs);
		// 	_TypeBase::operator=(rhs);

		// 	return *this;
		// }

		IntType& operator=(IntType&& rhs)
		{
		 	Json::operator=(std::forward<Json>(rhs));
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual bool operator==(const Json& rhs) const override
		{
			if (rhs.GetDataType() == DataTypes::Integer)
			{
				return static_cast<const _TypeBase&>(*this) == rhs.GetInt();
			}

			return false;
		}

		virtual bool IsNull() const override
		{
			return false;
		}

		virtual DataTypes GetDataType() const noexcept override
		{
			return DataTypes::Integer;
		}

		virtual const DefaultIntType& GetInt() const override
		{
			return GetVal();
		}

		virtual DefaultIntType& GetInt() override
		{
			return GetVal();
		}

		virtual void ToString(std::back_insert_iterator<std::string> dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const override
		{
			_TypeBase::ToString(dest, indent, lineEnd, sortKeys, precision, nestLevel, addComma);
		}

	protected:

		virtual const char* GetTypeName() const noexcept override
		{
			return "Integer";
		}
	};
}
