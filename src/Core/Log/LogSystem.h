#pragma once

#include <spdlog/spdlog.h>

#include <cstdint>
#include <stdexcept>

#define Info(...) instance().log(LogSystem::LogType::App, LogSystem::LogLevel::info, __VA_ARGS__);
#define Debug(...) instance().log(LogSystem::LogType::App, LogSystem::LogLevel::debug, __VA_ARGS__);
#define Elf_Error(...) instance().log(LogSystem::LogType::App, LogSystem::LogLevel::error, __VA_ARGS__);

#define SPW_ERROR(...) LogSystem::instance().log(LogSystem::LogType::App, LogSystem::LogLevel::error, __VA_ARGS__)
#define SPW_INFO(...) LogSystem::instance().log(LogSystem::LogType::App, LogSystem::LogLevel::info, __VA_ARGS__)
#define SPW_WARN(...) LogSystem::instance().log(LogSystem::LogType::App, LogSystem::LogLevel::warn, __VA_ARGS__)


class LogSystem final {
public:
    enum class LogLevel : uint8_t {
        debug,
        info,
        warn,
        error,
        fatal
    };

    enum class LogType : uint8_t {
        App,
        Scene,
        Editor
    };


    static LogSystem& instance()
    {
        static LogSystem instance;
        return instance;
    }

    LogSystem(const LogSystem&) = delete;
    LogSystem& operator=(const LogSystem&) = delete;
    
    template<typename... TARGS>
    void log(LogType type, LogLevel level, TARGS &&... args) {
        auto &logger = m_AppLogger;
        switch (type) {
            case LogType::Scene:
                logger = m_SceneLogger;
                break;
            case LogType::Editor:
                logger = m_EditorLogger;
                break;
            case LogType::App:
                break;
        }

        switch (level) {
            case LogLevel::debug:
                logger->debug(std::forward<TARGS>(args)...);
                break;
            case LogLevel::info:
                logger->info(std::forward<TARGS>(args)...);
                break;
            case LogLevel::warn:
                logger->warn(std::forward<TARGS>(args)...);
                break;
            case LogLevel::error:
                logger->error(std::forward<TARGS>(args)...);
                break;
            case LogLevel::fatal:
                logger->critical(std::forward<TARGS>(args)...);
                fatalCallback(std::forward<TARGS>(args)...);
                break;
            default:
                break;
        }
    }

    template<typename... TARGS>
    void fatalCallback(TARGS &&... args) {
        const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
        throw std::runtime_error(format_str);
    }

private:
    LogSystem();
    ~LogSystem();

    std::shared_ptr<spdlog::logger> m_AppLogger;
    std::shared_ptr<spdlog::logger> m_SceneLogger;
    std::shared_ptr<spdlog::logger> m_EditorLogger;

};