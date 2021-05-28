// Parser definition refers to: https://tools.ietf.org/id/draft-ietf-json-rfc4627bis-09.html
#pragma once

#include "Json.hpp"

#include "Internal/ParserHelpers.hpp"
#include "Internal/Utf.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	template<typename _StringCtnType = std::string>
	class String
	{
	public: // Static member:

		using StringCtnType = _StringCtnType;

		typedef typename StringCtnType::traits_type              traits_type;
		typedef typename StringCtnType::value_type               value_type;
		typedef typename StringCtnType::allocator_type           allocator_type;
		typedef typename StringCtnType::size_type                size_type;
		typedef typename StringCtnType::difference_type          difference_type;
		typedef typename StringCtnType::reference                reference;
		typedef typename StringCtnType::const_reference          const_reference;
		typedef typename StringCtnType::pointer                  pointer;
		typedef typename StringCtnType::const_pointer            const_pointer;
		typedef typename StringCtnType::iterator                 iterator;
		typedef typename StringCtnType::const_iterator           const_iterator;
		typedef typename StringCtnType::reverse_iterator         reverse_iterator;
		typedef typename StringCtnType::const_reverse_iterator   const_reverse_iterator;

		static const size_type npos = StringCtnType::npos;

		template<typename _rhs_StringCtnType>
		friend String<_rhs_StringCtnType> operator+(const StringCtnType& lhs, const String<_rhs_StringCtnType>& rhs);

		template<typename InputIt>
		static std::pair<String, InputIt> ParsePartial(InputIt begin, InputIt end, const InputIt oriPos)
		{
			StringCtnType resStr;

			if (Parser::Internal::NextChar(begin, end, oriPos) == '\"')
			{
				bool isEnd = false;
				while (!isEnd)
				{
					auto ch = Parser::Internal::ImmdPeekChar(begin, end, oriPos);
					if (ch == '\"') // Ending
					{
						++begin;
						isEnd = true;
					}
					else if (ch == '\\') // Escape something
					{
						++begin;
						ch = Parser::Internal::ImmdNextChar(begin, end, oriPos);
						switch (ch)
						{
						case '\"':
							resStr.push_back('\"');
							break;
						case '\\':
							resStr.push_back('\\');
							break;
						case '/':
							resStr.push_back('/');
							break;
						case 'b':
							resStr.push_back('\b');
							break;
						case 'f':
							resStr.push_back('\f');
							break;
						case 'n':
							resStr.push_back('\n');
							break;
						case 'r':
							resStr.push_back('\r');
							break;
						case 't':
							resStr.push_back('\t');
							break;

						case 'u':
							{
								char16_t firstCh = ParseEscapeUtf16Hex(begin, end, oriPos);
								ParseEscapeUtf16(firstCh, begin, end, oriPos, std::back_inserter(resStr));
							}
							break;

						default:
							throw ParseError("Unexpected character", oriPos, begin);
						}
					}
					else if (Internal::IsAscii(ch)) // ASCII char
					{
						++begin;
						resStr.push_back(ch);
					}
					else // Default - assuming UTF-8
					{
						try
						{
							begin = Internal::UtfConvertOnce(Internal::Utf8ToCodePtOnce<InputIt>,
								Internal::CodePtToUtf8Once<std::back_insert_iterator<StringCtnType> >,
								begin, end, std::back_inserter(resStr));
						}
						catch(const Internal::UtfConversionException& e)
						{
							throw ParseError(std::string("Invalid Unicode - ") + e.what(), oriPos, begin);
						}
					}
				}

				begin = Parser::Internal::SkipLeadingSpace(begin, end);
				return std::make_pair(
					std::move(resStr),
					begin
				);
			}

			throw ParseError("Unexpected character", oriPos, begin);
		}

		template<typename InputIt>
		static String Parse(InputIt begin, InputIt end)
		{
			auto partialRes = ParsePartial(begin, end, begin);

			if(partialRes.second != end)
			{
				throw ParseError("Extra Data", begin, partialRes.second);
			}

			return std::move(partialRes.first);
		}

	private:

		template<typename InputIt>
		static char16_t ParseEscapeUtf16Hex(InputIt& begin, InputIt end, const InputIt oriPos)
		{
			char16_t res = 0;
			for (size_t i = 0; i < 4; ++i)
			{
				res <<= 4;

				auto ch = Parser::Internal::ImmdNextChar(begin, end, oriPos);
				uint8_t tmp = 0;
				switch (ch)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					tmp = ch - '0';
					break;

				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					tmp = (ch - 'A') + 0xa;
					break;

				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
					tmp = (ch - 'a') + 0xa;
					break;

				default:
					throw ParseError("Invalid \\uXXXX escape", oriPos, begin - 1);
				}

				res |= tmp;
			}

			return res;
		}

		template<typename InputIt, typename OutputIt>
		static void ParseEscapeUtf16(char16_t first, InputIt& begin, InputIt end, const InputIt oriPos, OutputIt dest)
		{
			std::u16string utf16(1, first);

			auto tmpBegin = begin;
			while (Parser::Internal::ImmdNextChar(tmpBegin, end, oriPos) == '\\' &&
				Parser::Internal::ImmdNextChar(tmpBegin, end, oriPos) == 'u') // Do we have more UTF-16 to parse?
			{
				begin = tmpBegin;

				utf16.push_back(ParseEscapeUtf16Hex(begin, end, oriPos));

				tmpBegin = begin;
			}

			try
			{
				Internal::Utf16ToUtf8(utf16.begin(), utf16.end(), dest);
			}
			catch(const Internal::UtfConversionException& e)
			{
				throw ParseError(std::string("Invalid Unicode - ") + e.what(), oriPos, begin);
			}
		}

		template<typename OutputIt>
		static void ToStringUXXXXEscapeHex(char16_t val, OutputIt dest)
		{
			constexpr char alphabet[] = "0123456789ABCDEF";

			uint8_t nibble = 0;

			nibble = static_cast<uint8_t>((val >> 12) & 0x0F);
			std::copy(std::begin(alphabet) + nibble, std::begin(alphabet) + nibble + 1, dest);

			nibble = static_cast<uint8_t>((val >>  8) & 0x0F);
			std::copy(std::begin(alphabet) + nibble, std::begin(alphabet) + nibble + 1, dest);

			nibble = static_cast<uint8_t>((val >>  4) & 0x0F);
			std::copy(std::begin(alphabet) + nibble, std::begin(alphabet) + nibble + 1, dest);

			nibble = static_cast<uint8_t>((val >>  0) & 0x0F);
			std::copy(std::begin(alphabet) + nibble, std::begin(alphabet) + nibble + 1, dest);
		}

		template<typename InputIt, typename OutputIt>
		static InputIt ToStringUXXXXEscape(InputIt begin, InputIt end, OutputIt dest)
		{
			constexpr char escStr[] = "\\u";

			std::u16string tmpUtf6;

			begin = Internal::Utf8ToUtf16Once(begin, end, std::back_inserter(tmpUtf6));

			ToStringUXXXXEscapeHex(tmpUtf6.front(), dest);

			for (size_t i = 1; i < tmpUtf6.size(); ++i)
			{
				std::copy(std::begin(escStr), std::end(escStr) - 1, dest);
				ToStringUXXXXEscapeHex(tmpUtf6[i], dest);
			}

			return begin;
		}

	public:
		String() :
			m_data()
		{}

		String(const_pointer other) :
			m_data(other)
		{}

		String(const StringCtnType& other) :
			m_data(other)
		{}

		String(StringCtnType&& other) :
			m_data(std::forward<StringCtnType>(other))
		{}

		// Deep Copy
		// String(const String& other) :
		// 	m_data(other.m_data)
		// {}

		String(String&& other) :
			m_data(std::move(other.m_data))
		{}

		virtual ~String()
		{}

		// Deep Copy
		// String& operator=(const String& rhs)
		// {
		// 	m_data = rhs.m_data;

		// 	return *this;
		// }

		String& operator=(String&& rhs)
		{
			if (this != &rhs)
			{
				m_data = std::move(rhs.m_data);
			}

			return *this;
		}

		bool operator==(const String& rhs) const { return m_data == rhs.m_data; }
		bool operator!=(const String& rhs) const { return m_data != rhs.m_data; }
		bool operator<=(const String& rhs) const { return m_data <= rhs.m_data; }
		bool operator>=(const String& rhs) const { return m_data >= rhs.m_data; }
		bool operator<(const String& rhs) const { return m_data < rhs.m_data; }
		bool operator>(const String& rhs) const { return m_data > rhs.m_data; }

		String operator+(const String& str) { return String(m_data + str.m_data); }

		String& operator+=(const String& str) { m_data += str.m_data; return *this; }

		void swap(String& other) { return m_data.swap(other.m_data); }

		String& insert(size_type index, const String& str) { return m_data.insert(index, str.m_data); }
		String& insert(size_type index, const String& str,
			size_type index_str, size_type count = StringCtnType::npos) { return m_data.insert(index, str.m_data, index_str, count); }
		String& append(const String& str) { m_data.append(str.m_data); return *this; }
		String& append(const String& str,
			size_type pos, size_type count = StringCtnType::npos) { m_data.append(str.data, pos, count); return *this; }

		int compare(const StringCtnType& str) const noexcept { return m_data.compare(str); }
		int compare(size_type pos1, size_type count1,
			const StringCtnType& str) const { return m_data.compare(pos1, count1, str); }
		int compare(size_type pos1, size_type count1, const StringCtnType& str,
			size_type pos2, size_type count2 = npos) const { return m_data.compare(pos1, count1, str, pos2, count2); }

		String& operator=(const StringCtnType& rhs)
		{
			m_data = rhs;
			return *this;
		}

		String& operator=(StringCtnType&& rhs)
		{
			m_data = std::move(rhs);
			return *this;
		}

		bool operator==(const StringCtnType& rhs) const { return m_data == rhs; }
		bool operator!=(const StringCtnType& rhs) const { return m_data != rhs; }
		bool operator<=(const StringCtnType& rhs) const { return m_data <= rhs; }
		bool operator>=(const StringCtnType& rhs) const { return m_data >= rhs; }
		bool operator<(const StringCtnType& rhs) const { return m_data < rhs; }
		bool operator>(const StringCtnType& rhs) const { return m_data > rhs; }

		String operator+(const StringCtnType& str) { return String(m_data + str); }

		String& operator+=(const StringCtnType& str) { m_data += str; return *this; }
		String& operator+=(value_type ch) { m_data += ch; return *this; }
		String& operator+=(const_pointer s) { m_data += s; return *this; }
		String& operator+=(std::initializer_list<value_type> ilist) { m_data += ilist; return *this; }

		reference at(size_type pos) { return m_data.at(pos); }
		const_reference at(size_type pos) const { return m_data.at(pos); }

		reference operator[](size_type pos) { return m_data[pos]; }
		const_reference operator[](size_type pos) const { return m_data[pos]; }

		reference front() { return m_data.front(); }
		const_reference front() const { return m_data.front(); }
		reference back() { return m_data.back(); }
		const_reference back() const { return m_data.back(); }
		pointer data() noexcept { return m_data.data(); }
		const_pointer data() const noexcept { return m_data.data(); }
		const_pointer c_str() const noexcept { return m_data.c_str(); }

		iterator begin() noexcept { return m_data.begin(); }
		const_iterator begin() const noexcept { return m_data.begin(); }
		iterator end() noexcept { return m_data.end(); }
		const_iterator end() const noexcept { return m_data.end(); }

		reverse_iterator rbegin() noexcept { return m_data.rbegin(); }
		const_reverse_iterator rbegin() const noexcept { return m_data.rbegin(); }
		reverse_iterator rend() noexcept { return m_data.rend(); }
		const_reverse_iterator rend() const noexcept { return m_data.rend(); }

		const_iterator cbegin() const noexcept { return m_data.cbegin(); }
		const_iterator cend() const noexcept { return m_data.cend(); }
		const_reverse_iterator crbegin() const noexcept { return m_data.crbegin(); }
		const_reverse_iterator crend() const noexcept { return m_data.crend(); }

		size_type size() const noexcept { return m_data.size(); }
		size_type capacity() const noexcept { return m_data.capacity(); }
		bool empty() const noexcept { return m_data.empty(); }
		void reserve(size_type new_cap) { return m_data.reserve(new_cap); }
		void resize(size_type count) { return m_data.resize(count); }
		void resize(size_type count, value_type val) { return m_data.resize(count, val); }
		void shrink_to_fit() { return m_data.shrink_to_fit(); }

		void push_back(value_type value) { return m_data.push_back(value); }
		void pop_back() { return m_data.pop_back(); }

		void clear() noexcept { return m_data.clear(); }

		String substr(size_type pos = 0, size_type count = npos) const { return String(m_data.substr(pos, count)); }

		size_type copy(pointer dest, size_type count, size_type pos = 0) const { return m_data.copy(dest, count, pos); }

		void swap(StringCtnType& other) { return m_data.swap(other); }

		String& insert(size_type index, size_type count, value_type ch) { m_data.insert(index, count, ch); return *this; }
		String& insert(size_type index, const_pointer s) { m_data.insert(index, s); return *this; }
		String& insert(size_type index, const_pointer s, size_type count) { m_data.insert(index, s, count); return *this; }
		String& insert(size_type index, const StringCtnType& str) { m_data.insert(index, str); return *this; }
		String& insert(size_type index, const StringCtnType& str,
			size_type index_str, size_type count = npos) { m_data.insert(index, str, index_str, count); return *this; }
		iterator insert(const_iterator pos, value_type ch) { return m_data.insert(pos, ch); }
		iterator insert(const_iterator pos, size_type count, value_type ch) { return m_data.insert(pos, count, ch); }
		template<class InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) { return m_data.insert(pos, first, last); }
		iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) { return m_data.insert(pos, ilist); }

		String& append(size_type count, value_type ch) { m_data.append(count, ch); return *this; }
		String& append(const StringCtnType& str) { m_data.append(str); return *this; }
		String& append(const StringCtnType& str,
			size_type pos, size_type count = npos) { m_data.append(str, pos, count); return *this; }
		String& append(const_pointer s, size_type count) { m_data.append(s, count); return *this; }
		String& append(const_pointer s) { m_data.append(s); return *this; }
		template<class InputIt>
		String& append(InputIt first, InputIt last) { m_data.append(first, last); return *this; }
		String& append(std::initializer_list<value_type> ilist) { m_data.append(ilist); return *this; }

		int compare(const String& str) const noexcept { return m_data.compare(str.m_data); }
		int compare(size_type pos1, size_type count1,
			const String& str) const { return m_data.compare(pos1, count1, str.m_data); }
		int compare(size_type pos1, size_type count1, const String& str,
			size_type pos2, size_type count2 = npos) const { return m_data.compare(pos1, count1, str.m_data, pos2, count2); }
		int compare(const_pointer s) const { return m_data.compare(s); }
		int compare(size_type pos1, size_type count1, const_pointer s) const { return m_data.compare(pos1, count1, s); }
		int compare(size_type pos1, size_type count1, const_pointer s,
			size_type count2) const { return m_data.compare(pos1, count1, s, count2); }

		String& replace(size_type pos, size_type count,
			const StringCtnType& str) { m_data.replace(pos, count, str); return *this; }
		String& replace(const_iterator first, const_iterator last,
			const StringCtnType& str) { m_data.replace(first, last, str); return *this; }
		String& replace(size_type pos, size_type count, const StringCtnType& str,
			size_type pos2, size_type count2 = npos) { m_data.replace(pos, count, str, pos2, count2); return *this; }

		String& replace(size_type pos, size_type count,
			const String& str) { m_data.replace(pos, count, str.m_data); return *this; }
		String& replace(const_iterator first, const_iterator last,
			const String& str) { m_data.replace(first, last, str.m_data); return *this; }
		String& replace(size_type pos, size_type count, const String& str,
			size_type pos2, size_type count2 = npos) { m_data.replace(pos, count, str.m_data, pos2, count2); return *this; }
		template<class InputIt>
		String& replace(const_iterator first, const_iterator last,
			InputIt first2, InputIt last2){ m_data.replace(first, last, first2, last2); return *this; }
		String& replace(size_type pos, size_type count,
			const_pointer cstr, size_type count2) { m_data.replace(pos, count, cstr, count2); return *this; }
		String& replace(const_iterator first, const_iterator last,
			const_pointer cstr, size_type count2) { m_data.replace(first, last, cstr, count2); return *this; }
		String& replace(size_type pos, size_type count,
			const_pointer cstr) { m_data.replace(pos, count, cstr); return *this; }
		String& replace(const_iterator first, const_iterator last,
			const_pointer cstr) { m_data.replace(first, last, cstr); return *this; }
		String& replace(size_type pos, size_type count,
			size_type count2, value_type ch) { m_data.replace(pos, count, count2, ch); return *this; }
		String& replace(const_iterator first, const_iterator last,
			size_type count2, value_type ch) { m_data.replace(first, last, count2, ch); return *this; }
		String& replace(const_iterator first, const_iterator last,
			std::initializer_list<value_type> ilist) { m_data.replace(first, last, ilist); return *this; }

		size_type find(const StringCtnType& str, size_type pos = 0) const noexcept { return m_data.find(str, pos); }
		size_type find(const String& str, size_type pos = 0) const noexcept { return m_data.find(str.m_data, pos); }
		size_type find(const_pointer s, size_type pos, size_type count) const { return m_data.find(s, pos, count); }
		size_type find(const_pointer s, size_type pos = 0) const  { return m_data.find(s, pos); }
		size_type find(value_type ch, size_type pos = 0) const noexcept { return m_data.find(ch, pos); }

		size_type rfind(const StringCtnType& str,
			size_type pos = npos) const noexcept { return m_data.rfind(str, pos); }
		size_type rfind(const String& str,
			size_type pos = npos) const noexcept { return m_data.rfind(str.m_data, pos); }
		size_type rfind(const_pointer s, size_type pos, size_type count) const { return m_data.rfind(s, pos, count); }
		size_type rfind(const_pointer s, size_type pos = npos) const { return m_data.rfind(s, pos); }
		size_type rfind(value_type ch, size_type pos = npos) const noexcept { return m_data.rfind(ch, pos); }

		size_type find_first_of(const StringCtnType& str,
			size_type pos = 0) const noexcept { return m_data.find_first_of(str, pos); }
		size_type find_first_of(const String& str,
			size_type pos = 0) const noexcept { return m_data.find_first_of(str.m_data, pos); }
		size_type find_first_of(const_pointer s, size_type pos, size_type count) const { return m_data.find_first_of(s, pos, count); }
		size_type find_first_of(const_pointer s, size_type pos = 0) const { return m_data.find_first_of(s, pos); }
		size_type find_first_of(value_type ch, size_type pos = 0) const noexcept { return m_data.find_first_of(ch, pos); }

		size_type find_first_not_of(const StringCtnType& str,
			size_type pos = 0) const noexcept { return m_data.find_first_not_of(str, pos); }
		size_type find_first_not_of(const String& str,
			size_type pos = 0) const noexcept { return m_data.find_first_not_of(str.m_data, pos); }
		size_type find_first_not_of(const_pointer s,
			size_type pos, size_type count) const { return m_data.find_first_not_of(s, pos, count); }
		size_type find_first_not_of(const_pointer s,
			size_type pos = 0) const { return m_data.find_first_not_of(s, pos); }
		size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept { return m_data.find_first_not_of(ch, pos); }

		size_type find_last_of(const StringCtnType& str,
			size_type pos = npos) const { return m_data.find_last_of(str, pos); }
		size_type find_last_of(const String& str,
			size_type pos = npos) const { return m_data.find_last_of(str.m_data, pos); }
		size_type find_last_of(const_pointer s,
			size_type pos, size_type count) const { return m_data.find_last_of(s, pos, count); }
		size_type find_last_of(const_pointer s, size_type pos = npos) const { return m_data.find_last_of(s, pos); }
		size_type find_last_of(value_type ch, size_type pos = npos) const noexcept { return m_data.find_last_of(ch, pos); }

		size_type find_last_not_of(const StringCtnType& str,
			size_type pos = npos) const noexcept { return m_data.find_last_not_of(str, pos); }
		size_type find_last_not_of(const String& str,
			size_type pos = npos) const noexcept { return m_data.find_last_not_of(str.m_data, pos); }
		size_type find_last_not_of(const_pointer s,
			size_type pos, size_type count) const { return m_data.find_last_not_of(s, pos, count); }
		size_type find_last_not_of(const_pointer s, size_type pos = npos) const { return m_data.find_last_not_of(s, pos); }
		size_type find_last_not_of(value_type ch, size_type pos = npos) const noexcept { return m_data.find_last_not_of(ch, pos); }

		StringCtnType& GetUtf8String()
		{
			return m_data;
		}

		const StringCtnType& GetUtf8String() const
		{
			return m_data;
		}

		template<typename OutputIt>
		void ToString(OutputIt dest,
			const std::string& indent = "", const std::string& lineEnd = "\n",
			bool sortKeys = false, size_t precision = gk_defaultRealPrecision,
			size_t nestLevel = 0, bool addComma = false) const
		{
			constexpr char quoteStr[] = "\"";
			constexpr char escStr[] = "\\";
			constexpr char slsStr[] = "/";
			constexpr char bStr[] = "b";
			constexpr char fStr[] = "f";
			constexpr char nStr[] = "n";
			constexpr char rStr[] = "r";
			constexpr char tStr[] = "t";
			constexpr char uStr[] = "u";

			constexpr char cmaStr[] = ",";

			std::copy(std::begin(quoteStr), std::end(quoteStr) - 1, dest);

			for (auto it = m_data.begin(); it != m_data.end();)
			{
				if (Internal::IsPrintableAscii(*it) && (*it) != '\"' && (*it) != '\\' && (*it) != '/')
				// output directly
				{
					std::copy(it, it + 1, dest);
					it++;
				}
				else
				// needs escape
				{
					std::copy(std::begin(escStr), std::end(escStr) - 1, dest);

					switch (*it)
					{
					case '\"':
						std::copy(std::begin(quoteStr), std::end(quoteStr) - 1, dest);
						it++;
						break;
					case '\\':
						std::copy(std::begin(escStr), std::end(escStr) - 1, dest);
						it++;
						break;
					case '/':
						std::copy(std::begin(slsStr), std::end(slsStr) - 1, dest);
						it++;
						break;
					case '\b':
						std::copy(std::begin(bStr), std::end(bStr) - 1, dest);
						it++;
						break;
					case '\f':
						std::copy(std::begin(fStr), std::end(fStr) - 1, dest);
						it++;
						break;
					case '\n':
						std::copy(std::begin(nStr), std::end(nStr) - 1, dest);
						it++;
						break;
					case '\r':
						std::copy(std::begin(rStr), std::end(rStr) - 1, dest);
						it++;
						break;
					case '\t':
						std::copy(std::begin(tStr), std::end(tStr) - 1, dest);
						it++;
						break;
					default: // needs \uXXXX escape
						std::copy(std::begin(uStr), std::end(uStr) - 1, dest);
						it = ToStringUXXXXEscape(it, m_data.end(), dest);
						break;
					}
				}
			}

			std::copy(std::begin(quoteStr), std::end(quoteStr) - 1, dest);

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
		StringCtnType m_data;
	};

	template<typename _StringCtnType>
	inline bool operator==(const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs == lhs; }
	template<typename _StringCtnType>
	inline bool operator!=(const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs != lhs; }
	template<typename _StringCtnType>
	inline bool operator<=(const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs >= lhs; }
	template<typename _StringCtnType>
	inline bool operator>=(const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs <= lhs; }
	template<typename _StringCtnType>
	inline bool operator< (const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs >  lhs; }
	template<typename _StringCtnType>
	inline bool operator> (const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return rhs <  lhs; }

	template<typename _StringCtnType>
	inline String<_StringCtnType> operator+(const _StringCtnType& lhs, const String<_StringCtnType>& rhs) { return String<_StringCtnType>(lhs + rhs.m_data); }

	class StringType : public Json, public String<>
	{
	public: // Static members:

		using _TypeBase = String<>;

	public:
		StringType() :
			Json::Json(),
			_TypeBase::String()
		{}

		StringType(const char* val) :
			Json::Json(),
			_TypeBase::String(val)
		{}

		// Deep copy
		// StringType(const _TypeBase& val) :
		// 	Json::Json(),
		// 	_TypeBase::String(val)
		// {}

		StringType(_TypeBase&& val) :
			Json::Json(),
			_TypeBase::String(std::move(val))
		{}

		// Deep copy
		// StringType(const StringType& other) :
		// 	Json::Json(other),
		// 	_TypeBase::String(other)
		// {}

		StringType(StringType&& other) :
			Json::Json(std::forward<Json>(other)),
			_TypeBase::String(std::forward<_TypeBase>(other))
		{}

		virtual ~StringType()
		{}

		StringType& operator=(_TypeBase&& rhs)
		{
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		// Deep Copy
		// StringType& operator=(const StringType& rhs)
		// {
		// 	Json::operator=(rhs);
		// 	_TypeBase::operator=(rhs);

		// 	return *this;
		// }

		StringType& operator=(StringType&& rhs)
		{
			Json::operator=(std::forward<Json>(rhs));
			_TypeBase::operator=(std::forward<_TypeBase>(rhs));

			return *this;
		}

		virtual bool operator==(const StringType& rhs) const
		{
			return static_cast<const _TypeBase&>(*this) == static_cast<const _TypeBase&>(rhs.GetString());
		}

		virtual bool operator==(const Json& rhs) const override
		{
			if (rhs.GetDataType() == DataTypes::String)
			{
				return (*this) == rhs.GetString();
			}

			return false;
		}

		virtual StringType& GetString()
		{
			return (*this);
		}

		virtual const StringType& GetString() const
		{
			return (*this);
		}

		virtual std::string& GetUtf8String()
		{
			return _TypeBase::GetUtf8String();
		}

		virtual const std::string& GetUtf8String() const
		{
			return _TypeBase::GetUtf8String();
		}

		virtual bool IsNull() const override
		{
			return false;
		}

		virtual DataTypes GetDataType() const noexcept override
		{
			return DataTypes::String;
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
			return "String";
		}
	};
}

namespace std
{
#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
	template<typename _StringCtnType>
	struct hash<SimpleJson::String<_StringCtnType> > : std::hash<_StringCtnType> {};
#else
	template<typename _StringCtnType>
	struct hash<SIMPLEJSON_CUSTOMIZED_NAMESPACE::String<_StringCtnType> > : std::hash<_StringCtnType> {};
#endif
}
