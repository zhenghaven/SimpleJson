#pragma once

#include <vector>

#include "Json.hpp"
#include "ParserHelpers.hpp"
#include "CntValProxy.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	namespace Internal
	{
		template<typename DataType>
		using VectorType = std::vector<DataType>;
	}

	template<typename DataType, template<typename> typename _ContainerType = Internal::VectorType>
	class Vector
	{
	public: // Static member:

		typedef  _ContainerType<DataType>                      _cnt_type;

		typedef  typename _cnt_type::value_type                value_type;

		typedef  typename _cnt_type::allocator_type            allocator_type;

		typedef  typename _cnt_type::size_type                 size_type;
		typedef  typename _cnt_type::difference_type           difference_type;
		typedef  typename _cnt_type::reference                 reference;
		typedef  typename _cnt_type::const_reference           const_reference;
		typedef  typename _cnt_type::pointer                   pointer;
		typedef  typename _cnt_type::const_pointer             const_pointer;

		typedef  typename _cnt_type::iterator                  iterator;
		typedef  typename _cnt_type::const_iterator            const_iterator;
		typedef  typename _cnt_type::const_reverse_iterator    const_reverse_iterator;
		typedef  typename _cnt_type::reverse_iterator          reverse_iterator;

		template<typename InputIt>
		static std::pair<Vector, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			begin = Parser::SkipLeadingSpace(begin, end);

			if (begin != end && *begin == '[')
			{
				++begin;
				begin = Parser::SkipLeadingSpace(begin, end);

				Vector res;
				bool isEnd = false;

				while(!isEnd)
				{
					if (begin == end)
					{
						throw ParseError("Unexpected ending", oriPos, begin);
					}
					else if (*begin == ']')
					{
						++begin;
						isEnd = true;
					}
					else
					{
						DataType val;

						std::tie(val, begin) = DataType::ParsePartial(begin, end, oriPos);

						res.emplace_back(std::move(val));

						begin = Parser::SkipLeadingSpace(begin, end);

						if (begin == end)
						{
							throw ParseError("Unexpected ending", oriPos, begin);
						}
						else if (*begin == ',')
						{
							++begin;
						}
						else if (*begin == ']')
						{
							++begin;
							isEnd = true;
						}
						else
						{
							throw ParseError("Unexpected string", oriPos, begin);
						}
					}
					begin = Parser::SkipLeadingSpace(begin, end);
				}

				return std::make_pair(
					std::move(res),
					begin
				);
			}

			throw ParseError("Unexpected string", oriPos, begin);
		}

		template<typename InputIt>
		static Vector Parse(InputIt begin, InputIt end)
		{
			Vector res;
			InputIt it;

			std::tie(res, it) = ParsePartial(begin, end, begin);

			if(it != end)
			{
				throw ParseError("Extra Data", begin, it);
			}

			return res;
		}

	public:

		Vector() :
			m_data()
		{}

		Vector(Vector&& other) :
			m_data(std::move(other.m_data))
		{}

		virtual ~Vector()
		{}

		Vector& operator=(Vector&& rhs)
		{
			if (this != &rhs)
			{
				m_data = std::move(rhs.m_data);
			}

			return *this;
		}

		bool operator==(const Vector& rhs) const
		{
			return m_data == rhs.m_data;
		}

		bool operator!=(const Vector& rhs) const
		{
			return m_data != rhs.m_data;
		}

		void swap(Vector& other) noexcept
		{
			m_data.swap(other.m_data);
		}

		void clear() noexcept
		{
			m_data.clear();
		}

		iterator begin() noexcept
		{ return m_data.begin(); }

		const_iterator begin() const noexcept
		{ return m_data.begin(); }

		iterator end() noexcept
		{ return m_data.end(); }

		const_iterator end() const noexcept
		{ return m_data.end(); }

		reverse_iterator rbegin() noexcept
		{ return m_data.rbegin(); }

		const_reverse_iterator rbegin() const noexcept
		{ return m_data.rbegin(); }

		reverse_iterator rend() noexcept
		{ return m_data.rend(); }

		const_reverse_iterator rend() const noexcept
		{ return m_data.rend(); }

		const_iterator cbegin() const noexcept
		{ return m_data.cbegin(); }

		const_iterator cend() const noexcept
		{ return m_data.cend(); }

		const_reverse_iterator crbegin() const noexcept
		{ return m_data.crbegin(); }

		const_reverse_iterator crend() const noexcept
		{ return m_data.crend(); }

		reference operator[](size_type n)
		{
			return m_data.operator[](n);
		}

		const_reference operator[](size_type n) const
		{
			return m_data.operator[](n);
		}

		reference at(size_type pos)
		{
			return m_data.at(pos);
		}

		const_reference at(size_type pos) const
		{
			return m_data.at(pos);
		}

		size_type size() const noexcept
		{
			return m_data.size();
		}

		size_type capacity() const noexcept
		{
			return m_data.capacity();
		}

		bool empty() const noexcept
		{
			return m_data.empty();
		}
		void reserve(size_type new_cap)
		{
			return m_data.reserve(new_cap);
		}

		void resize(size_type count)
		{
			return m_data.resize(count);
		}

		void resize(size_type count, const value_type& value)
		{
			return m_data.resize(count, value);
		}

		void shrink_to_fit()
		{
			return m_data.shrink_to_fit();
		}

		void push_back(const value_type& value)
		{
			return m_data.push_back(value);
		}

		void push_back(value_type&& value)
		{
			return m_data.push_back(std::forward<value_type>(value));
		}

		void pop_back()
		{
			return m_data.pop_back();
		}

		template<typename... _Args>
#if __cplusplus > 201402L
		reference
#else
		void
#endif
		emplace_back(_Args&&... args)
		{
			return m_data.emplace_back(std::forward<_Args>(args)...);
		}

		template< class... _Args >
		iterator emplace(const_iterator pos, _Args&&... args)
		{
			return m_data.emplace(pos, std::forward<_Args>(args)...);
		}

		iterator insert(const_iterator pos, const value_type& value)
		{
			return m_data.insert(pos, value);
		}

		iterator insert(const_iterator pos, value_type&& value)
		{
			return m_data.insert(pos, std::forward<value_type>(value));
		}

		iterator insert(iterator pos, size_type count, const value_type& value)
		{
			return m_data.insert(pos, count, value);
		}

		iterator insert(const_iterator pos, size_type count, const value_type& value)
		{
			return m_data.insert(pos, count, value);
		}

		template<typename _InputIt>
		iterator insert(iterator pos, _InputIt first, _InputIt last)
		{
			return m_data.insert(pos, first, last);
		}

		template<class _InputIt>
		iterator insert(const_iterator pos, _InputIt first, _InputIt last)
		{
			return m_data.insert(pos, first, last);
		}

		iterator erase(iterator pos)
		{
			return m_data.erase(pos);
		}

		iterator erase(const_iterator pos)
		{
			return m_data.erase(pos);
		}

		iterator erase(iterator first, iterator last)
		{
			return m_data.erase(first, last);
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			return m_data.erase(first, last);
		}

		void assign(size_type count, const value_type& value)
		{
			return m_data.assign(count, value);
		}

		template<class _InputIt>
		void assign(_InputIt first, _InputIt last)
		{
			return m_data.assign(first, last);
		}

		std::string ToString(const std::string& indent = "", const std::string& lineEnd = "\n", bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const
		{
			if (indent.size() > 0)
			{
				if (m_data.size() > 0)
				{
					std::string appIndent;
					for (size_t i = 0; i < nestLevel; ++i)
					{
						appIndent += indent;
					}

					std::string res = '[' + lineEnd;

					for (size_t i = 0; i < m_data.size() - 1; ++i)
					{
						res += appIndent + indent + m_data[i].ToString(indent, lineEnd, sortKeys, nestLevel + 1, true);
					}

					res += appIndent + indent + m_data.back().ToString(indent, lineEnd, sortKeys, nestLevel + 1, false);

					res += appIndent + ']' + (addComma ? "," : "") + lineEnd;

					return res;
				}
				else
				{
					return std::string("[]") + (addComma ? "," : "") + lineEnd;
				}
			}
			else
			{
				std::string res = "[";

				for (size_t i = 0; m_data.size() > 0 && i < m_data.size() - 1; ++i)
				{
					res += m_data[i].ToString(indent, lineEnd, sortKeys, nestLevel + 1, true);
				}

				if (m_data.size() > 0)
				{
					res += m_data.back().ToString(indent, lineEnd, sortKeys, nestLevel + 1, false);
				}

				res = res + ']' + (addComma ? "," : "");

				return res;
			}
		}

	private:

		_cnt_type m_data;
	};

	class ArrayType : public Json, public Vector<CntValProxy>
	{
	public: // Static members:

		using _TypeBase = Vector<CntValProxy>;

	public:
		ArrayType() :
			Json::Json(),
			_TypeBase::Vector()
		{}

		ArrayType(ArrayType&& other) :
			Json::Json(),
			_TypeBase::Vector(std::forward<_TypeBase>(other))
		{}

		ArrayType(_TypeBase val) :
			Json::Json(),
			_TypeBase::Vector(std::move(val))
		{}

		virtual ~ArrayType()
		{}

		ArrayType& operator=(_TypeBase&& rhs)
		{
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual const ArrayType& GetArray() const override
		{
			return *this;
		}

		virtual ArrayType& GetArray() override
		{
			return *this;
		}

		virtual bool operator==(const Json& rhs) const override
		{
			if (rhs.GetDataType() == DataTypes::Array)
			{
				return *this == rhs.GetArray();
			}

			return false;
		}

		virtual bool IsNull() const override
		{
			return false;
		}

		virtual DataTypes GetDataType() const noexcept override
		{
			return DataTypes::Array;
		}

		virtual std::string ToString(const std::string& indent = "", const std::string& lineEnd = "\n", bool sortKeys = false, size_t nestLevel = 0, bool addComma = false) const override
		{
			return _TypeBase::ToString(indent, lineEnd, sortKeys, nestLevel, addComma);
		}

	protected:

		virtual const char* GetTypeName() const noexcept override
		{
			return "Array";
		}
	};
}
