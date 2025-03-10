#include "ConfigManager.h"
#include "FileSystem/FileDefinitions.h"
#include "FileSystem/FileSystem.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace Sparrow
{
    void ConfigManager::Initialize()
    {
        LoadConfig();
    }

    void ConfigManager::LoadConfig()
    {
        try
        {
            std::ifstream file(k_ConfigFileName);
            if (!file.is_open())
            {
                LOG_FATAL(LogModule::Asset, "Failed to open config file {}", k_ConfigFileName);
                throw std::runtime_error("Failed to open config file");
            }

            nlohmann::json config;
            file >> config;
            file.close();

            // 只读取路径信息
            const auto& directories = config["directories"];
            m_EngineConfig.m_ConfigRootDirectory   = directories["config"]["path"];
            m_EngineConfig.m_InstallRootDirectory  = directories["assemble"]["path"];
            m_EngineConfig.m_AssetRootDirectory    = directories["asset"]["path"];
            m_EngineConfig.m_ShaderRootDirectory   = directories["shader"]["path"];

            LOG_INFO(LogModule::Asset, "ConfigRootDirectory  : {}", m_EngineConfig.m_ConfigRootDirectory);
            LOG_INFO(LogModule::Asset, "InstallRootDirectory : {}", m_EngineConfig.m_InstallRootDirectory);
            LOG_INFO(LogModule::Asset, "AssetRootDirectory   : {}", m_EngineConfig.m_AssetRootDirectory);
            LOG_INFO(LogModule::Asset, "ShaderRootDirectory  : {}", m_EngineConfig.m_ShaderRootDirectory);

        }
        catch (const std::exception& e)
        {
            LOG_FATAL(LogModule::Asset, "Failed to load config file {}", k_ConfigFileName);
            throw std::runtime_error("Failed to load config file: " + String(e.what()));
        }
    }

    void ConfigManager::SaveConfig()
    {
        // TODO:
    }
} 