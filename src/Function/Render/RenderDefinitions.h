#pragma once

#include "glad/glad.h"

constexpr int RGBA = 4;

enum class OpenGLTextureWrapMode : GLint {
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
    Nearest = GL_NEAREST
};


enum class TextureType {
    COLOR, // Texture2D
    DEPTH,
    DEPTHARRAY,
    CUBE,   // TextureCube
    OTHER
};

enum class DrawCallType {
    ELE_TRIANGLE,
    ELE_PATCH,
    ARR_TRIANGLE,
    ARR_PATCH
};

struct TextureData {
    unsigned char *data = nullptr;
    int width{0};
    int height{0};
    int channels{0};
};
