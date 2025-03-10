#pragma once

#include <Core/Core.h>
#include <filesystem>

namespace Sparrow
{
    enum class FileOpenFlags
    {
        READ,
        WRITE,
        READ_WRITE,
        WRITE_READ
    };

    // FileExtensions
    enum class FileExtensions
    {
        GLTF,
        FBX,
        PNG,
        BVH,
        JSON,
        // ...
    };

    enum class FileTreeType
    {
        ENGINE,
        PROJECT,
        SHADER,
        SCRIPT,
        DEFAULT
    };

    const std::map<FileExtensions, String> k_FileExtensionsMap =
    {
        {FileExtensions::GLTF, ".gltf"},
        {FileExtensions::FBX, ".fbx"},
        {FileExtensions::PNG, ".png"},
        {FileExtensions::BVH, ".bvh"},
        {FileExtensions::JSON, ".json"},
        // ...
    };

    struct FileMetaInfo
    {
        String name;
        String fullPath;
    };

    struct DirectoryMetaInfo
    {
        String name;
        String fullPath;
        Vector<DirectoryMetaInfo> subDirectories;
        Vector<FileMetaInfo> files;
    };

    struct FileTree
    {
        FileTreeType type{FileTreeType::DEFAULT}; /* type of the file tree*/
        DirectoryMetaInfo mountPoint{}; /* virutal path dir */
        DirectoryMetaInfo resolvePoint{}; /* real path dir */
    };
}
