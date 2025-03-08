/** @file  version.h
 *  @brief Functions to query the version of the Assimp runtime, check
 *    compile flags, ...
 */
#pragma once
#ifndef AI_VERSION_H_INC
#define AI_VERSION_H_INC

#include <assimp/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
/** @brief Returns a string with legal copyright and licensing information
 *  about Assimp. The string may include multiple lines.
 *  @return Pointer to static string.
 */
ASSIMP_API const char*  aiGetLegalString  (void);

// ---------------------------------------------------------------------------
/** @brief Returns the current patch version number of Assimp.
 *  @return Patch version of the Assimp runtime the application was
 *    linked/built against
 */
ASSIMP_API unsigned int aiGetVersionPatch(void);

// ---------------------------------------------------------------------------
/** @brief Returns the current minor version number of Assimp.
 *  @return Minor version of the Assimp runtime the application was
 *    linked/built against
 */
ASSIMP_API unsigned int aiGetVersionMinor (void);

// ---------------------------------------------------------------------------
/** @brief Returns the current major version number of Assimp.
 *  @return Major version of the Assimp runtime the application was
 *    linked/built against
 */
ASSIMP_API unsigned int aiGetVersionMajor (void);

// ---------------------------------------------------------------------------
/** @brief Returns the repository revision of the Assimp runtime.
 *  @return SVN Repository revision number of the Assimp runtime the
 *          application was linked/built against.
 */
ASSIMP_API unsigned int aiGetVersionRevision (void);

// ---------------------------------------------------------------------------
/** @brief Returns the branch-name of the Assimp runtime.
 *  @return The current branch name.
 */
ASSIMP_API const char *aiGetBranchName();

//! Assimp was compiled as a shared object (Windows: DLL)
#define ASSIMP_CFLAGS_SHARED  0x1
//! Assimp was compiled against STLport
#define ASSIMP_CFLAGS_STLPORT 0x2
//! Assimp was compiled as a debug build
#define ASSIMP_CFLAGS_DEBUG   0x4

//! Assimp was compiled with ASSIMP_BUILD_BOOST_WORKAROUND defined
#define ASSIMP_CFLAGS_NOBOOST           0x8
//! Assimp was compiled with ASSIMP_BUILD_SINGLETHREADED defined
#define ASSIMP_CFLAGS_SINGLETHREADED    0x10
//! Assimp was compiled with ASSIMP_BUILD_SINGLETHREADED defined
#define ASSIMP_CFLAGS_DOUBLE_SUPPORT 0x20

// ---------------------------------------------------------------------------
/** @brief Returns assimp's compile flags
 *  @return Any bitwise combination of the ASSIMP_CFLAGS_xxx constants.
 */
ASSIMP_API unsigned int aiGetCompileFlags(void);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // !! #ifndef AI_VERSION_H_INC

