#pragma once

#include <Core/Core.h>
#include <glad/glad.h>

namespace Sparrow
{
    // Assert GL function return type value (signed != -1 && unsigned != 0u) // So genious -_- to not use unsigned everywhere // Metaprogramming == my love :)
#define GLASSERT(glretval) \
	static_assert(std::is_arithmetic_v<decltype(glretval)>); \
	if constexpr (std::is_signed_v    <decltype(glretval)>) { ASSERT(glretval != -1); } \
	else /* if (unsigned arithmetic type)                */ { ASSERT(glretval != 0u); }

#define GLCall(function_call) \
	while (glGetError() != GL_NO_ERROR) /* pop */;\
	function_call;\
	if (glGetError() != GL_NO_ERROR) {\
		std::cerr << __FILE__ << ':' << __LINE__ << ": " << #function_call << "\n\n";\
		BREAKPOINT(); } // <-- scope_end}
}
