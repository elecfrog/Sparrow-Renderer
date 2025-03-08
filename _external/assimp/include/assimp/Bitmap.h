/** @file Bitmap.h
 *  @brief Defines bitmap format helper for textures
 *
 * Used for file formats which embed their textures into the model file.
 */
#pragma once
#ifndef AI_BITMAP_H_INC
#define AI_BITMAP_H_INC

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include "defs.h"
#include <cstdint>
#include <cstddef>

struct aiTexture;

namespace Assimp {

class IOStream;

// ---------------------------------------------------------------------------
/**
 *  This class is used to store and write bitmap information.
 */
class ASSIMP_API Bitmap {
protected:

    struct Header {
        uint16_t type;
        uint32_t size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset;

        // We define the struct size because sizeof(Header) might return a wrong result because of structure padding.
        static constexpr std::size_t header_size =
            sizeof(uint16_t) +
            sizeof(uint32_t) +
            sizeof(uint16_t) +
            sizeof(uint16_t) +
            sizeof(uint32_t);
    };

    struct DIB {
        uint32_t size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        int32_t x_resolution;
        int32_t y_resolution;
        uint32_t nb_colors;
        uint32_t nb_important_colors;

        // We define the struct size because sizeof(DIB) might return a wrong result because of structure padding.
        static constexpr std::size_t dib_size =
            sizeof(uint32_t) +
            sizeof(int32_t) +
            sizeof(int32_t) +
            sizeof(uint16_t) +
            sizeof(uint16_t) +
            sizeof(uint32_t) +
            sizeof(uint32_t) +
            sizeof(int32_t) +
            sizeof(int32_t) +
            sizeof(uint32_t) +
            sizeof(uint32_t);
    };

    static constexpr std::size_t mBytesPerPixel = 4;

public:
    /// @brief  Will save an aiTexture instance as a bitmap.
    /// @param texture  The pointer to the texture instance
    /// @param file     The filename to save into.
    /// @return true if successfully saved, false if not.
    static bool Save(aiTexture* texture, IOStream* file);

protected:
    static void WriteHeader(Header& header, IOStream* file);
    static void WriteDIB(DIB& dib, IOStream* file);
    static void WriteData(aiTexture* texture, IOStream* file);
};

}

#endif // AI_BITMAP_H_INC
