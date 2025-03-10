#include "FileSystem.h"

#include <Config/ConfigManager.h>

#include "Engine/Engine.h"

namespace Sparrow
{
    std::string FileSystem::JoinFileRoute(const FilePath& directory, const std::string& fileRoute)
    {
        std::string ret = directory.string().append("/").append(fileRoute);
        ResolveSlash(ret);
        return ret;
    }

    fs::path FileSystem::GetAssetFilePath(const String& relativePath)
    {
        String asset_root = g_Engine.m_ConfigManager->GetAssetRoot();
        
        // 使用fs::path的构造函数来正确拼接路径
        fs::path path(asset_root);
        path /= relativePath;
        
        // 规范化路径并转换为字符串
        std::string path_str = path.lexically_normal().string();
        // 替换单反斜杠为双反斜杠
        std::replace(path_str.begin(), path_str.end(), '\\', '/');
        
        // 重新构造path对象
        path = fs::path(path_str);
        
        if (fs::is_regular_file(path))
        {
            return std::move(path);
        }

        std::string description = path.string() + " does not exist or is not a regular file.";
        LOG_FATAL(LogModule::Asset, "{}", description);
        throw std::runtime_error(description);
    }

    fs::path FileSystem::GetShaderFilePath(const String& relativePath)
    {
        String shader_root = g_Engine.m_ConfigManager->GetShaderRoot();
        
        // 使用fs::path的构造函数来正确拼接路径
        fs::path path(shader_root);
        path /= relativePath;
        
        // 规范化路径并转换为字符串
        std::string path_str = path.lexically_normal().string();
        // 替换单反斜杠为双反斜杠
        std::replace(path_str.begin(), path_str.end(), '\\', '/');
        
        // 重新构造path对象
        path = fs::path(path_str);
        
        if (fs::is_regular_file(path))
        {
            return std::move(path);
        }
        
        std::string description = path.string() + " does not exist or is not a regular file.";
        LOG_FATAL(LogModule::Asset, "{}", description);
        throw std::runtime_error(description);
    }
}
