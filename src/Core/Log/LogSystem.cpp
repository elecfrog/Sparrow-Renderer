#include "Core/Log/LogSystem.h"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

LogSystem::LogSystem()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%^%l%$] %v");

    const spdlog::sinks_init_list sink_list = { console_sink };

    spdlog::init_thread_pool(8192, 1);

    m_AppLogger = std::make_shared<spdlog::async_logger>("APP",
                                                         sink_list.begin(),
                                                         sink_list.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    m_AppLogger->set_level(spdlog::level::trace);

    spdlog::register_logger(m_AppLogger);

    m_SceneLogger = std::make_shared<spdlog::async_logger>("SCENE",
                                                         sink_list.begin(),
                                                         sink_list.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    m_SceneLogger->set_level(spdlog::level::trace);

    spdlog::register_logger(m_SceneLogger);

    m_EditorLogger = std::make_shared<spdlog::async_logger>("EDITOR",
                                                         sink_list.begin(),
                                                         sink_list.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    m_EditorLogger->set_level(spdlog::level::trace);

    spdlog::register_logger(m_EditorLogger);
}

LogSystem::~LogSystem()
{
    m_AppLogger->flush();
    m_SceneLogger->flush();
    m_EditorLogger->flush();
    spdlog::drop_all();
}