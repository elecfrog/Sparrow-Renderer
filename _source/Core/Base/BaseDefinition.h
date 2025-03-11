#pragma once

// STL common
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <array>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <utility>
#include <variant>
#include <memory>
#include <cstdint>
#include <iomanip>
#include <array>
#include <functional>
#include <atomic>
#include <thread>

#include <stdio.h>
#include <stdlib.h>

// glm math
#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Sparrow
{
    // Basic type aliases
    using Bool = bool;
    using Int = std::int32_t;
    using UInt = std::uint32_t;
    using Float = std::float_t;
    using Double = std::double_t;
    using String = std::string;
    using Vector2f = glm::vec2;
    using Vector3f = glm::vec3;
    using Vector4f = glm::vec4;
    using Matrix4f = glm::mat4;
    using Quaternion = glm::quat;


    using String = std::string;

    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T, typename... Args>
    UniquePtr<T> MakeUnique(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using SharedPtr = std::shared_ptr<T>;

    template <typename T>
    using Vector = std::vector<T>;

    // Common macros for null/false checks
#define RETURN_IF_NULL(EXPR)        if (!(EXPR)) return;
#define RETURN_IF_FALSE(EXPR)       if (!(EXPR)) return;
#define RETURN_NULL_IF_NULL(EXPR)   if (!(EXPR)) return nullptr;
#define RETURN_NULL_IF_FALSE(EXPR)  if (!(EXPR)) return nullptr;
#define RETURN_FALSE_IF_NULL(EXPR)  if (!(EXPR)) return false;
#define RETURN_FALSE_IF_FALSE(EXPR) if (!(EXPR)) return false;

#ifdef WIN32 // debug-breakpoint/platform-specific
#include <intrin.h>
#include <memory>
#define BREAKPOINT() __debugbreak()
#elif defined (__unix__)  || (defined (__APPLE__) && defined (__MACH__))
		#include <signal.h>
		#define BREAKPOINT() raise(SIGTRAP)
#else // see other methods: https://stackoverflow.com/questions/173618/is-there-a-portable-equivalent-to-debugbreak-debugbreak/49079078#49079078
    //#define BREAKPOINT() // uncomment if no solutions found
#endif // debug-breakpoint/platform-specific

    // Trigger debug breakpoint on `condition` fail
#ifdef _DEBUG
#define ASSERT(condition) if (!(condition)) BREAKPOINT()
#else
    #define ASSERT(condition) ((void)0)
#endif
}

namespace Sparrow
{
    template <typename K, typename V>
    using HashMap = std::unordered_map<K, V>;

    static inline Vector3f ToGLMVec3(const float _v[3])
    {
        return Vector3f(_v[0], _v[1], _v[2]);
    }
}
