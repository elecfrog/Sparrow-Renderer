#include "Core/Log/LogSystem.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

namespace Sparrow
{
    LogSystem::LogSystem()
    {
        std::filesystem::create_directories("logs");

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/sparrow.log", true);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        std::vector<spdlog::sink_ptr> sinks = {console_sink, file_sink};

        m_AppLogger = std::make_shared<spdlog::logger>("APP", sinks.begin(), sinks.end());
        m_SceneLogger = std::make_shared<spdlog::logger>("SCENE", sinks.begin(), sinks.end());
        m_EditorLogger = std::make_shared<spdlog::logger>("EDITOR", sinks.begin(), sinks.end());
        m_RenderLogger = std::make_shared<spdlog::logger>("RENDER", sinks.begin(), sinks.end());
        m_AssetLogger = std::make_shared<spdlog::logger>("ASSET", sinks.begin(), sinks.end());
        m_FbxConverterLogger = std::make_shared<spdlog::logger>("FBXCONV", sinks.begin(), sinks.end());

        // 设置日志级别
        spdlog::set_level(spdlog::level::trace);

        // 注册到spdlog
        spdlog::register_logger(m_AppLogger);
        spdlog::register_logger(m_SceneLogger);
        spdlog::register_logger(m_EditorLogger);
        spdlog::register_logger(m_RenderLogger);
        spdlog::register_logger(m_AssetLogger);
        spdlog::register_logger(m_FbxConverterLogger);
    }

    LogSystem::~LogSystem()
    {
        // 关闭并清理所有logger
        spdlog::shutdown();
    }
} // namespace Log
