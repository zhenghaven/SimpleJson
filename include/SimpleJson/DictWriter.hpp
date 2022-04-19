// Copyright (c) 2022 Haofan Zheng
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include "Internal/SimpleObjects.hpp"

#include "Utils.hpp"
#include "WriterConfig.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{

template<
	typename _KeyWriter,
	typename _ObjWriter,
	typename _ToStringType,
	typename _ContainerType>
struct JsonWriterDictImpl
{

	using DictBase =
		typename Internal::Obj::BaseObject<_ToStringType>::DictBase;

	template<typename _OutputIt>
	inline static void Write(_OutputIt destIt,
		const DictBase& obj,
		const WriterConfig& config,
		const WriterStates& state)
	{
		*destIt++ = '{';

		WriterStates stateNextLevel = state;
		++(stateNextLevel.m_nestLevel);
		size_t len = obj.size();

		if (config.m_indent.size() > 0 && obj.size() > 0)
		{
			Internal::RepeatOutput(destIt, config.m_lineEnd, 1);
		}

		for(auto it = obj.cbegin(); it != obj.cend(); ++it, --len)
		{
			if (config.m_indent.size() > 0)
			{
				Internal::RepeatOutput(destIt,
					config.m_indent, stateNextLevel.m_nestLevel);
			}

			_KeyWriter::Write(destIt,
				*(std::get<0>(*it)), config, stateNextLevel);

			if (config.m_indent.size() > 0)
			{
				*destIt++ = ' ';
				*destIt++ = ':';
				*destIt++ = ' ';
			}
			else
			{
				*destIt++ = ':';
			}

			_ObjWriter::Write(destIt,
				*(std::get<1>(*it)), config, stateNextLevel);

			if (len != 1)
			{
				*destIt++ = ',';
			}

			if (config.m_indent.size() > 0)
			{
				Internal::RepeatOutput(destIt, config.m_lineEnd, 1);
			}
		}

		if (config.m_indent.size() > 0 && obj.size() > 0)
		{
			Internal::RepeatOutput(destIt, config.m_indent, state.m_nestLevel);
		}
		*destIt++ = '}';
	}

}; // struct JsonWriterDictImpl

} // namespace SimpleJson
