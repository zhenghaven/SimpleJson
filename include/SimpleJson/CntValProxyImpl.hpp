#pragma once

#include "CntValProxy.hpp"

#include "NullType.hpp"
#include "BoolType.hpp"
#include "ArrayType.hpp"

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	inline CntValProxy::CntValProxy() :
		m_data(MakeUniquePtr::make_unique<NullType>())
	{}

	inline CntValProxy::CntValProxy(BoolType&& val) :
		m_data(MakeUniquePtr::make_unique<BoolType>(std::forward<BoolType>(val)))
	{}

	inline CntValProxy::CntValProxy(ArrayType&& val) :
		m_data(MakeUniquePtr::make_unique<ArrayType>(std::forward<ArrayType>(val)))
	{}

	inline CntValProxy& CntValProxy::operator=(BoolType&& val)
	{
		m_data = MakeUniquePtr::make_unique<BoolType>(std::forward<BoolType>(val));

		return *this;
	}

	inline CntValProxy& CntValProxy::operator=(ArrayType&& val)
	{
		m_data = MakeUniquePtr::make_unique<ArrayType>(std::forward<ArrayType>(val));

		return *this;
	}

	inline void CntValProxy::Nullify()
	{
		m_data = MakeUniquePtr::make_unique<NullType>();
	}
}
