#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

#include "Core/Utils/Utility.hpp"
#include "Core/Log/LogSystem.h"
#include "Function/ImGui/ImGuiManager.h"
#include "Platform/WindowSystem/WindowSystem.h"
#include "Platform/FileSystem/FileSystem.h"
#include "Resource/AssetManager.h"
#include "Tests/SceneManager.hpp"

// Scences Followed by https://learnopengl.com/
#include "Tests/Scene_BVHViewer.hpp"
#include "Tests/Scene_PBRTest.hpp"
#include "Tests/Scene_LoadModel.hpp"
// #include "Tests/Scene_Terrain.hpp"
#include "Tests/Scene06_ShadowMapping.hpp"
#include "Tests/Scene08_PointShadow.hpp"
#include "Tests/Scene_ClothSimulationSS1.hpp"
#include "Tests/Scene_ClothSimulationSS2.hpp"


class MenuPanel {
public:
    MenuPanel(SceneManager *sceneManager)
            : m_SceneManager(sceneManager) {}

    void OnImGuiRender() {

        if (ImGui::Begin("Scenes")) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", ImGui::GetIO().Framerate,
                        1000.0f / ImGui::GetIO().Framerate);

            for (const auto &scene: m_SceneManager->GetScenes()) {
                if (ImGui::Button(scene.first.c_str())) {
                    auto newScene = scene.second();
                    m_SceneManager->SetActiveScene(newScene);
                }
            }
            ImGui::End();
        }
    }

private:
    SceneManager *m_SceneManager;
};

class Application {

public:
    Application() = default;

    /* using builder pattern */
    void Run() {
        Init().
                MainLoop().
                CleanUp();
    }

    /* using builder pattern */
    Application &Init() {
        return InitWindow()
                .InitImGui()
                .PrintInfo()
                .InitScenes()
                .InitMenuPanel();
    }

    Application &MainLoop() {
        while (!m_WindowSystem->ShouldClose()) {
            GLCall(glClearColor(0.0f / 255.f, 0.0f / 255.f, 0.0f / 255.f, 1.0f))
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

            m_ImguiManager->Begin();

            // Render the menu panel
            m_MenuPanel->OnImGuiRender();
            const auto &activeScene = m_SceneManager->GetActiveScene();
            if (activeScene != nullptr) {
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

    Application &CleanUp() {
        m_ImguiManager->CleanUp();

        return *this;
    }

    [[nodiscard]] GLFWwindow *GetWindow() const { return m_WindowSystem->GetWindowHandle(); }

public:
    // Initialize and create a m_WindowSystem.
    Application &InitWindow() {
        m_WindowSystem = std::make_unique<WindowSystem>();
        m_WindowSystem->Init(WindowCreateInfo{960, 540, "Sparrow Renderer", false});

        return *this;
    }

    // Initialize and create an ui layout, which needs a window as swapchain.
    Application &InitImGui() {
        m_ImguiManager = std::make_unique<elf::ImGuiManager>();
        m_ImguiManager->Init(m_WindowSystem->GetWindowHandle());
        return *this;
    }

    Application &PrintInfo() {
        LogSystem::Info("OpenGL : {0}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
        LogSystem::Info("GPU    : {0}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
        LogSystem::Info("ImGui  : {0}", reinterpret_cast<const char *>(IMGUI_VERSION));
#ifdef IMGUI_HAS_VIEWPORT && IMGUI_HAS_DOCK
        LogSystem::Info("ImGui  : {0} {1}", " +viewport", " +docking");
#endif
        return *this;
    }

    Application &InitScenes() {
        m_SceneManager = std::make_unique<SceneManager>(m_WindowSystem.get());
        // m_SceneManager->RegisterScene<Scene01_Triangle>("OpenGL Triangle");
        m_SceneManager->RegisterScene<Scene_BVHViewer>("BVH Animation Viewer");
        m_SceneManager->RegisterScene<Scene_PBRTest>("PBR Test");
        m_SceneManager->RegisterScene<Scene_LoadModel>("Forward+");
        m_SceneManager->RegisterScene<Scene_PBRTest>("PBR Test");
        // m_SceneManager->RegisterScene<Scene_Terrain>("Terrain");
        m_SceneManager->RegisterScene<Scene06_ShadowMapping>("Shadow Mapping");
        m_SceneManager->RegisterScene<Scene_ClothSimulationSS1>("Cloth Simulation SS1");
        m_SceneManager->RegisterScene<Scene_ClothSimulationSS2>("Cloth Simulation SS2");
        // ...
        return *this;
    }

    Application &InitMenuPanel() {
        m_MenuPanel = std::make_unique<MenuPanel>(m_SceneManager.get());
        return *this;
    }

private:
    std::unique_ptr<WindowSystem> m_WindowSystem;
    std::unique_ptr<elf::ImGuiManager> m_ImguiManager;
    std::unique_ptr<SceneManager> m_SceneManager;
    std::unique_ptr<MenuPanel> m_MenuPanel;
};