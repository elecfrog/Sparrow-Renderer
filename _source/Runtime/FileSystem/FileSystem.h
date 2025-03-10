#pragma once

#include "FileDefinitions.h"
#include <Core/Core.h>

#include <nfd.h>
#include "imgui.h"

namespace Sparrow
{
    
    namespace fs = std::filesystem;
    using FilePath = fs::path;
    #define AssetPath(x)  FileSystem::GetAssetFilePath(x)
    #define ShaderPath(x) FileSystem::GetShaderFilePath(x)

    class FileSystem
    {
    public:
        static std::string JoinFileRoute(const FilePath& directory, const std::string& fileRoute);

        static void ResolveSlash(std::string& str)
        {
            std::replace(begin(str), end(str), '\\', '/');
        }

        static std::string GetFileExtension(const std::string& path) /* get a specific extension of a file */
        {
            const FilePath ret = path.c_str();
            return ret.extension().string();
        }

        static fs::path GetAssetFilePath(const String& relativePath);
        static fs::path GetShaderFilePath(const String& relativePath);


    private:
        std::unordered_map<std::string, FileTree> m_FileTrees;
    };
}
