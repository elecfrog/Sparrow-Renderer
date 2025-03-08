#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace Sparrow
{
    // Basic type aliases
    using Bool     = bool;
    using Int      = std::int32_t;
    using Float    = std::float_t;
    using Double   = std::double_t;
    using Vector2f = glm::vec2;
    using Vector3f = glm::vec3;
    using Vector4f = glm::vec4;
    using Matrix4f = glm::mat4;

    // Common macros for null/false checks
    #define RETURN_IF_NULL(EXPR)        if (!(EXPR)) return;
    #define RETURN_IF_FALSE(EXPR)       if (!(EXPR)) return;
    #define RETURN_NULL_IF_NULL(EXPR)   if (!(EXPR)) return nullptr;
    #define RETURN_NULL_IF_FALSE(EXPR)  if (!(EXPR)) return nullptr;
    #define RETURN_FALSE_IF_NULL(EXPR)  if (!(EXPR)) return false;
    #define RETURN_FALSE_IF_FALSE(EXPR) if (!(EXPR)) return false;
} 