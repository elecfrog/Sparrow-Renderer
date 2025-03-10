#pragma once

#include <Core/Core.h>

namespace Sparrow
{
    struct EngineConfig
    {
        String m_ConfigRootDirectory      { }; // the directory of config file
        String m_InstallRootDirectory     { }; // the directory of installed files directory
        String m_AssetRootDirectory       { }; // the directory of asset files directory
        String m_ShaderRootDirectory      { }; // the directory of shaders files directory, always should be a subdirectory in asset directory. Just for convenience.
    };

    class ConfigManager
    {
    public:
        void Initialize();
        void LoadConfig();
        void SaveConfig();

        const String& GetConfigRoot() { return m_EngineConfig.m_ConfigRootDirectory; }
        const String& GetInstallRoot() { return m_EngineConfig.m_InstallRootDirectory; }
        const String& GetAssetRoot() { return m_EngineConfig.m_AssetRootDirectory; }
        const String& GetShaderRoot() { return m_EngineConfig.m_ShaderRootDirectory; }

        void SetConfigRoot(const String& path) { m_EngineConfig.m_ConfigRootDirectory = path; }
        void SeInstallRoot(const String& path) { m_EngineConfig.m_InstallRootDirectory = path; }
        void SetAssetRoot(const String& path) { m_EngineConfig.m_AssetRootDirectory = path; }
        void SetShaderRoot(const String& path) { m_EngineConfig.m_ShaderRootDirectory = path; }

    private:
        EngineConfig m_EngineConfig;
        static inline const String k_ConfigFileName = "./config.json";
    };
}
