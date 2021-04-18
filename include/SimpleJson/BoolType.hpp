#pragma once

#include "Json.hpp"
#include "ParserHelpers.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	class Bool
	{
	public: // Static member:

		template<typename InputIt>
		static std::pair<Bool, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			auto firstCh = Parser::NextChar(begin, end, oriPos);
			if (firstCh == 't')
			{
				if (Parser::NextChar(begin, end, oriPos) == 'r' &&
					Parser::NextChar(begin, end, oriPos) == 'u' &&
					Parser::NextChar(begin, end, oriPos) == 'e')
				{
					begin = Parser::SkipLeadingSpace(begin, end);

					return std::make_pair(
						Bool(true),
						begin
					);
				}
			}
			else if (firstCh == 'f')
			{
				if (Parser::NextChar(begin, end, oriPos) == 'a' &&
					Parser::NextChar(begin, end, oriPos) == 'l' &&
					Parser::NextChar(begin, end, oriPos) == 's' &&
					Parser::NextChar(begin, end, oriPos) == 'e')
				{
					begin = Parser::SkipLeadingSpace(begin, end);

					return std::make_pair(
						Bool(false),
						begin
					);
				}
			}

			throw ParseError("Unexpected character", oriPos, begin);
		}

		template<typename InputIt>
		static Bool Parse(InputIt begin, InputIt end)
		{
			Bool res;
			InputIt it;

			std::tie(res, it) = ParsePartial(begin, end, begin);

			if(it != end)
			{
				throw ParseError("Extra Data", begin, it);
			}

			return res;
		}

	public:
		Bool() :
			m_data()
		{}

		Bool(bool val) :
			m_data(val)
		{}

		Bool(const Bool& other) :
			m_data(other.m_data)
		{}

		Bool(Bool&& other) noexcept:
			m_data(std::move(other.m_data))
		{}

		virtual ~Bool()
		{}

		operator bool() const
		{
			return m_data;
		}

		Bool& operator=(const Bool& rhs)
		{
			m_data = rhs.m_data;

			return *this;
		}

		Bool& operator=(bool rhs)
		{
			m_data = rhs;

			return *this;
		}

		bool operator==(bool rhs) const
		{
			return m_data == rhs;
		}

		bool operator!=(bool rhs) const
		{
			return m_data != rhs;
		}

		template<typename OutputIt>
		void ToString(OutputIt dest, const std::string& indent = "", const std::string& lineEnd = "\n", bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const
		{
			constexpr char trStr[] = "true";
			constexpr char faStr[] = "false";
			constexpr char cmaStr[] = ",";

			if (static_cast<bool>(*this))
			{
				std::copy(std::begin(trStr), std::end(trStr) - 1, dest);
			}
			else
			{
				std::copy(std::begin(faStr), std::end(faStr) - 1, dest);
			}

			if (addComma)
			{
				std::copy(std::begin(cmaStr), std::end(cmaStr) - 1, dest);
			}

			if (indent.size() > 0)
			{
				std::copy(lineEnd.begin(), lineEnd.end(), dest);
			}
		}

	private:

		bool m_data;
	};

	inline bool operator==(bool lhs, const Bool& rhs)
	{
		return rhs == lhs;
	}

	inline bool operator!=(bool lhs, const Bool& rhs)
	{
		return rhs != lhs;
	}

	class BoolType : public Json, public Bool
	{
	public: // Static members:

		using _TypeBase = Bool;

	public:
		BoolType() = default;

		BoolType(bool val) :
			Json::Json(),
			_TypeBase::Bool(val)
		{}

		BoolType(_TypeBase val) :
			Json::Json(),
			_TypeBase::Bool(std::move(val))
		{}

		virtual ~BoolType()
		{}

		BoolType& operator=(_TypeBase&& rhs)
		{
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual bool operator==(const Json& rhs) const override
		{
			if (rhs.GetDataType() == DataTypes::Boolean)
			{
				return static_cast<bool>(*this) == rhs.GetBool();
			}

			return false;
		}

		virtual bool GetBool() const override
		{
			return static_cast<bool>(*this);
		}

		virtual bool IsNull() const override
		{
			return false;
		}

		virtual DataTypes GetDataType() const noexcept override
		{
			return DataTypes::Boolean;
		}

		virtual void ToString(std::back_insert_iterator<std::string> dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t nestLevel = 0,
			bool addComma = false) const override
		{
			_TypeBase::ToString(dest, indent, lineEnd, sortKeys, nestLevel, addComma);
		}

	protected:

		virtual const char* GetTypeName() const noexcept override
		{
			return "Boolean";
		}
	};
}
