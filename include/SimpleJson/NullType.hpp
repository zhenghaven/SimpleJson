#pragma once

#include "Json.hpp"

#include "Internal/ParserHelpers.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	class Null
	{
	public: // Static member:

		template<typename InputIt>
		static std::pair<Null, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			if (Parser::Internal::NextChar(begin, end, oriPos) == 'n' &&
				Parser::Internal::NextChar(begin, end, oriPos) == 'u' &&
				Parser::Internal::NextChar(begin, end, oriPos) == 'l' &&
				Parser::Internal::NextChar(begin, end, oriPos) == 'l')
			{
				begin = Parser::Internal::SkipLeadingSpace(begin, end);

				return std::make_pair(
					Null(),
					begin
				);
			}

			throw ParseError("Unexpected character", oriPos, begin);
		}

		template<typename InputIt>
		static Null Parse(InputIt begin, InputIt end)
		{
			Null res;
			InputIt it;

			std::tie(res, it) = ParsePartial(begin, end, begin);

			if(it != end)
			{
				throw ParseError("Extra Data", begin, it);
			}

			return res;
		}

	public:
		Null() = default;

		virtual ~Null()
		{}

		template<typename OutputIt>
		void ToString(OutputIt dest, const std::string& indent = "", const std::string& lineEnd = "\n", bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const
		{
			constexpr char nlStr[] = "null";
			constexpr char cmaStr[] = ",";

			std::copy(std::begin(nlStr), std::end(nlStr) - 1, dest);

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

	class NullType : public Json, public Null
	{
	public: // Static members:

		using _TypeBase = Null;

	public:
		NullType() = default;

		NullType(_TypeBase val) :
			Json::Json(),
			_TypeBase::Null(std::move(val))
		{}

		virtual ~NullType()
		{}

		NullType& operator=(_TypeBase&& rhs)
		{
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual bool operator==(const Json& rhs) const override
		{
			return rhs.GetDataType() == DataTypes::Null;
		}

		virtual bool IsNull() const override
		{
			return true;
		}

		virtual DataTypes GetDataType() const noexcept
		{
			return DataTypes::Null;
		}

		virtual void ToString(std::back_insert_iterator<std::string> dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const override
		{
			_TypeBase::ToString(dest, indent, lineEnd, sortKeys, nestLevel, addComma);
		}

	protected:

		virtual const char* GetTypeName() const noexcept override
		{
			return "Null";
		}

	};
}
