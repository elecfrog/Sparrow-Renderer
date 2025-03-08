#pragma once

#include "RHI/OpenGL.h"

namespace Sparrow
{
    constexpr Int RGBA = 4;

    enum class OpenGLTextureWrapMode : GLint {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
        Nearest = GL_NEAREST
    };

    enum class TextureType {
        COLOR,      // Texture2D
        DEPTH,
        DEPTHARRAY,
        CUBE,       // TextureCube
        OTHER
    };

    enum class DrawCallType {
        ELE_TRIANGLE,
        ELE_PATCH,
        ARR_TRIANGLE,
        ARR_PATCH
    };

    struct TextureData {
        unsigned char* data = nullptr;
        Int width{0};
        Int height{0};
        Int channels{0};
    };
}
