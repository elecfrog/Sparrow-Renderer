#pragma once

#include <spdlog/spdlog.h>
#include <format>
#include <source_location>
#include <string_view>

namespace Sparrow {

enum class LogLevel : uint8_t {
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

enum class LogModule : uint8_t {
    App,
    Scene,
    Editor,
    Render,
    Asset,
    Fbx
};

class LogSystem final {
public:
    static LogSystem& instance() {
        static LogSystem instance;
        return instance;
    }

    LogSystem(const LogSystem&) = delete;
    LogSystem& operator=(const LogSystem&) = delete;

    template<typename... Args>
    static void logMessage(LogLevel level, 
                         LogModule module,
                         const std::string_view file,
                         const uint32_t line,
                         std::format_string<Args...> fmt,
                         Args&&... args) {
        auto& logger = instance().getLogger(module);
        const auto formatted_msg = std::format(fmt, std::forward<Args>(args)...);
        const auto location_info = std::format("{}:{}", file, line);

        switch (level) {
            case LogLevel::Debug:
                logger->debug("[{}] {}", location_info, formatted_msg);
                break;
            case LogLevel::Info:
                logger->info("[{}] {}", location_info, formatted_msg);
                break;
            case LogLevel::Warn:
                logger->warn("[{}] {}", location_info, formatted_msg);
                break;
            case LogLevel::Error:
                logger->error("[{}] {}", location_info, formatted_msg);
                break;
            case LogLevel::Fatal:
                logger->critical("[{}] {}", location_info, formatted_msg);
                fatalCallback(formatted_msg);
                break;
        }
    }

private:
    LogSystem();
    ~LogSystem();

    std::shared_ptr<spdlog::logger>& getLogger(LogModule module) {
        switch (module) {
            case LogModule::Scene:
                return m_SceneLogger;
            case LogModule::Editor:
                return m_EditorLogger;
            case LogModule::Render:
                return m_RenderLogger;
            case LogModule::Asset:
                return m_AssetLogger;
            case LogModule::Fbx:
                return m_FbxConverterLogger;
            default:
                return m_AppLogger;
        }
    }

    static void fatalCallback(const std::string_view& message) {
        throw std::runtime_error(std::string(message));
    }

    std::shared_ptr<spdlog::logger> m_AppLogger;
    std::shared_ptr<spdlog::logger> m_SceneLogger;
    std::shared_ptr<spdlog::logger> m_EditorLogger;
    std::shared_ptr<spdlog::logger> m_RenderLogger;
    std::shared_ptr<spdlog::logger> m_AssetLogger;
    std::shared_ptr<spdlog::logger> m_FbxConverterLogger;
};

} // namespace Sparrow

#define LOG_DEBUG(module, fmt, ...) \
    Sparrow::LogSystem::logMessage(Sparrow::LogLevel::Debug, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(module, fmt, ...) \
    Sparrow::LogSystem::logMessage(Sparrow::LogLevel::Info, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARN(module, fmt, ...) \
    Sparrow::LogSystem::logMessage(Sparrow::LogLevel::Warn, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(module, fmt, ...) \
    Sparrow::LogSystem::logMessage(Sparrow::LogLevel::Error, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_FATAL(module, fmt, ...) \
    Sparrow::LogSystem::logMessage(Sparrow::LogLevel::Fatal, module, __FILE__, __LINE__, fmt, ##__VA_ARGS__) 