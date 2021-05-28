#pragma once

#include "Json.hpp"

#include "Internal/ParserHelpers.hpp"
#include "Internal/NumberParser.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	template<typename _ValType>
	class Number
	{
	public: // Static member:

		using NumValType = _ValType;

		static Number FromComponents(const std::string& intStr,
			const std::string& fracStr,
			const std::string& expStr)
		{
			return Number(Parser::Internal::ComponentsToNumber<NumValType>(intStr, fracStr, expStr));
		}

		template<typename InputIt>
		static std::pair<Number, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			std::string intStr;
			std::string fracStr;
			std::string expStr;

			Parser::Internal::ParseNumberSignInplace(begin, end, oriPos,
				std::back_inserter(intStr), std::back_inserter(fracStr), std::back_inserter(expStr));

			return std::make_pair(FromComponents(intStr, fracStr, expStr),
				begin);
		}

		template<typename InputIt>
		static Number Parse(InputIt begin, InputIt end)
		{
			Number res;
			InputIt it;

			std::tie(res, it) = ParsePartial(begin, end, begin);

			if(it != end)
			{
				throw ParseError("Extra Data", begin, it);
			}

			return res;
		}

	public:
		Number() :
			m_data()
		{}

		Number(const NumValType& val) :
			m_data(val)
		{}

		Number(NumValType&& val) :
			m_data(std::forward<NumValType>(val))
		{}

		Number(const Number& other) :
			m_data(other.m_data)
		{}

		Number(Number&& other) noexcept:
			m_data(std::move(other.m_data))
		{}

		virtual ~Number()
		{}

		Number& operator=(const Number& rhs)
		{
			m_data = rhs.m_data;

			return *this;
		}

		Number& operator=(Number&& rhs)
		{
			m_data = std::forward<NumValType>(rhs.m_data);

			return *this;
		}

		Number& operator=(const NumValType& rhs)
		{
			m_data = rhs;

			return *this;
		}

		Number& operator=(NumValType&& rhs)
		{
			m_data = std::forward<NumValType>(rhs);

			return *this;
		}

		bool operator==(const Number& rhs) const
		{
			return m_data == rhs.m_data;
		}

		bool operator!=(const Number& rhs) const
		{
			return m_data != rhs.m_data;
		}

		bool operator<=(const Number& rhs) const
		{
			return m_data <= rhs.m_data;
		}

		bool operator>=(const Number& rhs) const
		{
			return m_data >= rhs.m_data;
		}

		bool operator<(const Number& rhs) const
		{
			return m_data < rhs.m_data;
		}

		bool operator>(const Number& rhs) const
		{
			return m_data > rhs.m_data;
		}

		bool operator==(const NumValType& rhs) const
		{
			return m_data == rhs;
		}

		bool operator!=(const NumValType& rhs) const
		{
			return m_data != rhs;
		}

		bool operator<=(const NumValType& rhs) const
		{
			return m_data <= rhs;
		}

		bool operator>=(const NumValType& rhs) const
		{
			return m_data >= rhs;
		}

		bool operator<(const NumValType& rhs) const
		{
			return m_data < rhs;
		}

		bool operator>(const NumValType& rhs) const
		{
			return m_data > rhs;
		}

		Number operator+(const Number& rhs) const
		{
			return Number(m_data + rhs.m_data);
		}

		Number operator-(const Number& rhs) const
		{
			return Number(m_data - rhs.m_data);
		}

		Number operator*(const Number& rhs) const
		{
			return Number(m_data * rhs.m_data);
		}

		Number operator/(const Number& rhs) const
		{
			return Number(m_data / rhs.m_data);
		}

		Number operator%(const Number& rhs) const
		{
			return Number(m_data % rhs.m_data);
		}

		Number& operator+=(const Number& rhs)
		{
			m_data += rhs.m_data;
			return *this;
		}

		Number& operator-=(const Number& rhs)
		{
			m_data -= rhs.m_data;
			return *this;
		}

		Number& operator*=(const Number& rhs)
		{
			m_data *= rhs.m_data;
			return *this;
		}

		Number& operator/=(const Number& rhs)
		{
			m_data /= rhs.m_data;
			return *this;
		}

		Number& operator%=(const Number& rhs)
		{
			m_data %= rhs.m_data;
			return *this;
		}

		Number& operator++()
		{
			++m_data;
			return *this;
		}

		Number operator++(int)
		{
			return Number(m_data++);
		}

		Number& operator--()
		{
			--m_data;
			return *this;
		}

		Number operator--(int)
		{
			return Number(m_data--);
		}

		Number operator-() const
		{
			return Number(-m_data);
		}

		Number operator+(const NumValType& rhs) const
		{
			return Number(m_data + rhs);
		}

		Number operator-(const NumValType& rhs) const
		{
			return Number(m_data - rhs);
		}

		Number operator*(const NumValType& rhs) const
		{
			return Number(m_data * rhs);
		}

		Number operator/(const NumValType& rhs) const
		{
			return Number(m_data / rhs);
		}

		Number operator%(const NumValType& rhs) const
		{
			return Number(m_data % rhs);
		}

		Number& operator+=(const NumValType& rhs)
		{
			m_data += rhs;
			return *this;
		}

		Number& operator-=(const NumValType& rhs)
		{
			m_data -= rhs;
			return *this;
		}

		Number& operator*=(const NumValType& rhs)
		{
			m_data *= rhs;
			return *this;
		}

		Number& operator/=(const NumValType& rhs)
		{
			m_data /= rhs;
			return *this;
		}

		Number& operator%=(const NumValType& rhs)
		{
			m_data %= rhs;
			return *this;
		}

		NumValType& GetVal()
		{
			return m_data;
		}

		const NumValType& GetVal() const
		{
			return m_data;
		}

	private:

		NumValType m_data;
	};

	template<typename _NumValType>
	bool operator==(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs == lhs;
	}

	template<typename _NumValType>
	bool operator!=(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs != lhs;
	}

	template<typename _NumValType>
	bool operator<=(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs >= lhs;
	}

	template<typename _NumValType>
	bool operator>=(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs <= lhs;
	}

	template<typename _NumValType>
	bool operator<(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs > lhs;
	}

	template<typename _NumValType>
	bool operator>(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return rhs < lhs;
	}

	template<typename _NumValType>
	Number<_NumValType> operator+(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return Number<_NumValType>(rhs + lhs);
	}

	template<typename _NumValType>
	Number<_NumValType> operator-(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return Number<_NumValType>(lhs - rhs.GetVal());
	}

	template<typename _NumValType>
	Number<_NumValType> operator*(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return Number<_NumValType>(rhs * lhs);
	}

	template<typename _NumValType>
	Number<_NumValType> operator/(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return Number<_NumValType>(lhs / rhs.GetVal());
	}

	template<typename _NumValType>
	Number<_NumValType> operator%(const _NumValType& lhs, const Number<_NumValType>& rhs)
	{
		return Number<_NumValType>(lhs % rhs.GetVal());
	}
}
