/**
 *  @file
 *  @brief Default file I/O using fXXX()-family of functions
 */
#pragma once
#ifndef AI_DEFAULTIOSTREAM_H_INC
#define AI_DEFAULTIOSTREAM_H_INC

#ifdef __GNUC__
#   pragma GCC system_header
#endif

#include <cstdio>
#include <assimp/IOStream.hpp>
#include <assimp/importerdesc.h>

namespace Assimp {

// ----------------------------------------------------------------------------------
//! @class  DefaultIOStream
//! @brief  Default IO implementation, use standard IO operations
//! @note   An instance of this class can exist without a valid file handle
//!         attached to it. All calls fail, but the instance can nevertheless be
//!         used with no restrictions.
class ASSIMP_API DefaultIOStream : public IOStream {
    friend class DefaultIOSystem;
#if __ANDROID__
# if __ANDROID_API__ > 9
#  if defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
    friend class AndroidJNIIOSystem;
#  endif // defined(AI_CONFIG_ANDROID_JNI_ASSIMP_MANAGER_SUPPORT)
# endif // __ANDROID_API__ > 9
#endif // __ANDROID__

protected:
    /// @brief
    DefaultIOStream() AI_NO_EXCEPT;

    /// @brief The class constructor with the file name and the stream.
    /// @param pFile        The file-streaam
    /// @param strFilename  The file name
    DefaultIOStream(FILE* pFile, const std::string &strFilename);

public:
    /** Destructor public to allow simple deletion to close the file. */
    ~DefaultIOStream ();

    // -------------------------------------------------------------------
    /// Read from stream
    size_t Read(void* pvBuffer, size_t pSize, size_t pCount) override;

    // -------------------------------------------------------------------
    /// Write to stream
    size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) override;

    // -------------------------------------------------------------------
    /// Seek specific position
    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;

    // -------------------------------------------------------------------
    /// Get current seek position
    size_t Tell() const override;

    // -------------------------------------------------------------------
    /// Get size of file
    size_t FileSize() const override;

    // -------------------------------------------------------------------
    /// Flush file contents
    void Flush() override;

private:
    FILE* mFile;
    std::string mFilename;
    mutable size_t mCachedSize;
};

// ----------------------------------------------------------------------------------
AI_FORCE_INLINE DefaultIOStream::DefaultIOStream() AI_NO_EXCEPT :
        mFile(nullptr),
        mFilename(),
        mCachedSize(SIZE_MAX) {
    // empty
}

// ----------------------------------------------------------------------------------
AI_FORCE_INLINE DefaultIOStream::DefaultIOStream (FILE* pFile, const std::string &strFilename) :
        mFile(pFile),
        mFilename(strFilename),
        mCachedSize(SIZE_MAX) {
    // empty
}

// ----------------------------------------------------------------------------------

} // ns assimp

#endif //!!AI_DEFAULTIOSTREAM_H_INC
