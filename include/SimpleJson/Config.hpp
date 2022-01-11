#pragma once

#include <cstddef>

#ifndef SIMPLEJSON_LIBUTF_NAMESPACE
namespace SimpleUtf
#else
namespace SIMPLEJSON_LIBUTF_NAMESPACE;
#endif
{}

#ifndef SIMPLEJSON_CUSTOMIZED_NAMESPACE
namespace SimpleJson
#else
namespace SIMPLEJSON_CUSTOMIZED_NAMESPACE
#endif
{
	using DefaultIntType = int64_t;
	using DefaultRealType = double;

	constexpr size_t gk_defaultRealPrecision = 17;
	constexpr size_t gk_defaultPrintfBufSize = 36;

	constexpr bool gk_defaultSortKeys = false;

	namespace Internal
	{
#ifndef SIMPLEJSON_LIBUTF_NAMESPACE
		namespace Utf = SimpleUtf;
#else
		namespace Utf = SIMPLEJSON_LIBUTF_NAMESPACE;
#endif
	}
}
