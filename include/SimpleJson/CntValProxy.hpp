#pragma once

#include <memory>

#include "Json.hpp"

#include "GeneralTypeParser.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	class NullType;
	class BoolType;
	class ArrayType;

	class CntValProxy
	{
	public: // Static members:

		template<typename InputIt>
		static std::pair<CntValProxy, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			std::unique_ptr<Json> obj;

			std::tie(obj, begin) = Parser::GeneralTypeParser<InputIt>().ParsePartial(begin, end, oriPos);

			return std::make_pair(
				CntValProxy(std::move(obj)),
				begin
			);
		}

		template<typename InputIt>
		static CntValProxy Parse(InputIt begin, InputIt end)
		{
			CntValProxy res;
			InputIt oriPos = begin;

			std::tie(res, begin) = ParsePartial(begin, end, oriPos);

			if(begin != end)
			{
				throw ParseError("Extra Data", oriPos, begin);
			}

			return res;
		}

	public:

		CntValProxy();

		CntValProxy(const CntValProxy& other) = delete;

		CntValProxy(CntValProxy&& other) :
			m_data(std::move(other.m_data))
		{
			other.Nullify();
		}

		CntValProxy(std::unique_ptr<Json> other):
			m_data(std::move(other))
		{}

		//CntValProxy(NullType&& val);
		CntValProxy(BoolType&& val);
		CntValProxy(ArrayType&& val);

		virtual ~CntValProxy()
		{}

		CntValProxy& operator=(const CntValProxy& val) = delete;

		CntValProxy& operator=(CntValProxy&& val)
		{
			if (this != &val)
			{
				m_data = std::move(val.m_data);

				val.Nullify();
			}

			return *this;
		}

		//CntValProxy& operator=(NullType&& val);
		CntValProxy& operator=(BoolType&& val);
		CntValProxy& operator=(ArrayType&& val);

		Json& operator*()
		{
			return *m_data;
		}

		Json* operator->() noexcept
		{
			return m_data.operator->();
		}

		bool operator==(const CntValProxy& rhs) const
		{
			return *m_data == *rhs.m_data;
		}

		bool operator==(const Json& rhs) const
		{
			return *m_data == rhs;
		}

		bool operator!=(const CntValProxy& rhs) const
		{
			return *m_data != *rhs.m_data;
		}

		bool operator!=(const Json& rhs) const
		{
			return *m_data != rhs;
		}

		void Nullify();

		std::string ToString(
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			return m_data->ToString(indent, lineEnd, sortKeys, precision, nestLevel, addComma);
		}

		void ToString(std::back_insert_iterator<std::string> dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			return m_data->ToString(dest, indent, lineEnd, sortKeys, precision, nestLevel, addComma);
		}

		template<typename OutputIt>
		void ToString(OutputIt dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false,  size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			std::string tmp = m_data->ToString(indent, lineEnd, sortKeys, precision, nestLevel, addComma);
			std::copy(tmp.begin(), tmp.end(), dest);
		}

	private:
		std::unique_ptr<Json> m_data;
	};

	inline bool operator==(const Json& lhs, const CntValProxy& rhs)
	{
		return rhs == lhs;
	}

	inline bool operator!=(const Json& lhs, const CntValProxy& rhs)
	{
		return rhs != lhs;
	}
}
