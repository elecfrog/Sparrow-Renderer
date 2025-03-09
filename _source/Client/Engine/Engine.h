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

        class WindowSystem* m_WindowSystem;
        class ImGuiManager* m_ImguiManager;
        class SceneManager* m_SceneManager;
    };

    extern Engine g_Engine;
}
