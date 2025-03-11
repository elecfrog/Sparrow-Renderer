#include "Engine.h"

#include <Config/ConfigManager.h>

#include "Core/Log/LogSystem.h"
#include "WindowSystem/WindowSystem.h"
#include "Scene/SceneManager.hpp"
#include "ImGui/ImGuiManager.h"
#include "Render/RenderManager.h"
#include "System/MeshRenderSystem.h"
#include "System/MeshSystem.h"

namespace Sparrow
{
    Engine g_Engine;

    void Engine::Run()
    {
        Initialize().
        MainLoop().
        CleanUp();
    }

    Engine& Engine::Initialize()
    {
        LOG_INFO(LogModule::App, "Engine::Initialize Start");

        LOG_INFO(LogModule::App, "Engine::Initialize Config Start");
        m_ConfigManager = new ConfigManager();
        m_ConfigManager->Initialize();
        LOG_INFO(LogModule::App, "Engine::Initialize Config End");

        m_WindowSystem = new WindowSystem();
        m_WindowSystem->Init(WindowCreateInfo{960, 540, "Sparrow Renderer", false});

        m_ImguiManager = new ImGuiManager();
        m_ImguiManager->Init(m_WindowSystem->GetWindowHandle());

        PrintInfo();

        m_SceneManager = new SceneManager();
        m_SceneManager->RegisterScenes();

        m_RenderManager = new RenderManager();
        m_RenderManager->Initialize();

        m_MeshSystem       = new MeshSystem();
        m_MeshRenderSystem = new MeshRenderSystem();

        LOG_INFO(LogModule::App, "Engine::Initialize End");

        return *this;
    }

    Engine& Engine::MainLoop()
    {
        while (!m_WindowSystem->ShouldClose())
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f))
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

            m_ImguiManager->Begin();

            // Render the menu panel
            m_ImguiManager->Render();
            const auto& activeScene = m_SceneManager->GetActiveScene();
            if (activeScene != nullptr)
            {
                activeScene->OnUpdate();
                activeScene->OnRender();

                ImGui::Begin("Scene");
                activeScene->OnImGuiRender();
                ImGui::End();
            }
            // m_ImguiManager->GetProfilingPanel()->ClearItems();

            m_ImguiManager->End();
            m_ImguiManager->EnableViewport(m_WindowSystem->GetWindowHandle());

            m_WindowSystem->SwapBuffers();
            m_WindowSystem->PollEvents();
        } // while (!glfwWindowShouldClose(m_WindowSystem))
        return *this;
    }

    Engine& Engine::CleanUp()
    {
        delete m_RenderManager;

        m_ImguiManager->CleanUp();
        delete m_ImguiManager;

        delete m_WindowSystem;

        return *this;
    }

    Engine& Engine::PrintInfo()
    {
        LOG_INFO(LogModule::App, "OpenGL : {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        LOG_INFO(LogModule::App, "GPU    : {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        LOG_INFO(LogModule::App, "ImGui  : {0}", reinterpret_cast<const char*>(IMGUI_VERSION));
#ifdef IMGUI_HAS_VIEWPORT && IMGUI_HAS_DOCK
        LOG_INFO(LogModule::App, "ImGui  : {0} {1}", " +viewport", " +docking");
#endif
        return *this;
    }
}
