#pragma once

#include <cstdio>

#include "Number.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	template<typename _ValType = DefaultRealType>
	class Real : public Number<_ValType>
	{
	public: // Static member:

		using RealValType = _ValType;
		using _Base = Number<RealValType>;

		static Real FromComponents(const std::string& intStr,
			const std::string& fracStr,
			const std::string& expStr)
		{
			return Real(_Base::FromComponents(intStr, fracStr, expStr));
		}

		template<typename InputIt>
		static std::pair<Real, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			auto numRes = _Base::ParsePartial(begin, end, oriPos);

			return std::make_pair(
				Real(std::move(numRes.first)),
				numRes.second
			);
		}

		template<typename InputIt>
		static Real Parse(InputIt begin, InputIt end)
		{
			return Real(_Base::Parse(begin, end));
		}

	public:
		Real() :
			_Base::Number()
		{}

		Real(const RealValType& val) :
			_Base::Number(val)
		{}

		Real(RealValType&& val) :
			_Base::Number(std::forward<RealValType>(val))
		{}

		Real(_Base&& val) noexcept:
			_Base::Number(std::forward<_Base>(val))
		{}

		Real(const Real& other) :
			_Base::Number(other)
		{}

		Real(Real&& other) noexcept:
			_Base::Number(std::forward<_Base>(other))
		{}

		virtual ~Real()
		{}

		Real& operator=(const Real& rhs)
		{
			_Base::operator=(rhs);

			return *this;
		}

		Real& operator=(Real&& rhs)
		{
			_Base::operator=(std::forward<_Base>(rhs));

			return *this;
		}

		Real& operator=(const RealValType& rhs)
		{
			_Base::operator=(rhs);

			return *this;
		}

		Real& operator=(RealValType&& rhs)
		{
			_Base::operator=(std::forward<RealValType>(rhs));

			return *this;
		}

		template<typename OutputIt>
		void ToString(OutputIt dest, const
			std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			constexpr char cmaStr[] = ",";

			std::string realStr(gk_defaultPrintfBufSize, '\0');

			bool isWritten = false;
			while (!isWritten)
			{
				auto neededSize = std::snprintf(
					&realStr[0], realStr.size(),
					"%.*g",
					static_cast<int>(precision),
					_Base::GetVal()
				);
				if (static_cast<size_t>(neededSize) >= realStr.size())
				{
					realStr.resize(neededSize + 1);
				}
				else
				{
					isWritten = true;
					realStr.resize(neededSize);
				}
			}

			if (realStr.find('.') == std::string::npos &&
				realStr.find('e') == std::string::npos)
			{
				realStr += ".0";
			}

			std::copy(realStr.begin(), realStr.end(), dest);

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

	class RealType : public Json, public Real<>
	{
	public: // Static members:

		using _TypeBase = Real<>;

	public:
		RealType() :
			Json::Json(),
			_TypeBase::Real()
		{}

		RealType(const DefaultRealType& val) :
			Json::Json(),
			_TypeBase::Real(val)
		{}

		RealType(DefaultRealType&& val) :
			Json::Json(),
			_TypeBase::Real(std::forward<DefaultRealType>(val))
		{}

		// RealType(const RealType& other) :
		// 	Json::Json(other),
		// 	_TypeBase::Real(other)
		// {}

		RealType(RealType&& other) noexcept:
			Json::Json(std::forward<Json>(other)),
			_TypeBase::Real(std::forward<_TypeBase>(other))
		{}

		RealType(_TypeBase val) :
			Json::Json(),
			_TypeBase::Real(std::move(val))
		{}

		virtual ~RealType()
		{}

		// RealType& operator=(const RealType& rhs)
		// {
		// 	Json::operator=(rhs);
		// 	_TypeBase::operator=(rhs);

		// 	return *this;
		// }

		RealType& operator=(RealType&& rhs)
		{
		 	Json::operator=(std::forward<Json>(rhs));
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual bool operator==(const Json& rhs) const override
		{
			if (rhs.GetDataType() == DataTypes::Real)
			{
				return static_cast<const _TypeBase&>(*this) == rhs.GetReal();
			}

			return false;
		}

		virtual bool IsNull() const override
		{
			return false;
		}

		virtual DataTypes GetDataType() const noexcept override
		{
			return DataTypes::Real;
		}

		virtual const DefaultRealType& GetReal() const override
		{
			return GetVal();
		}

		virtual DefaultRealType& GetReal() override
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
			return "Real";
		}
	};
}
