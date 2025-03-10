#pragma once

namespace Sparrow
{
    class Engine
    {
    public:
        Engine() = default;

        void Run();

        Engine& Initialize();
        Engine& MainLoop();
        Engine& CleanUp();
        Engine& PrintInfo();

        class ConfigManager*  m_ConfigManager;
        class WindowSystem*   m_WindowSystem;
        class ImGuiManager*   m_ImguiManager;
        class SceneManager*   m_SceneManager;
        class RenderManager*  m_RenderManager;
    };

    extern Engine g_Engine;
}
