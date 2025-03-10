#include "ImGuiSceneMenu.h"
#include "Runtime/Scene/SceneManager.hpp"
#include "Runtime/Engine/Engine.h"

namespace Sparrow
{
    void ImGuiSceneMenu::Draw()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    ImGui::GetIO().Framerate,
                    1000.0f / ImGui::GetIO().Framerate);

        for (const auto& scene : g_Engine.m_SceneManager->GetScenes())
        {
            if (ImGui::Button(scene.first.c_str()))
            {
                auto newScene = scene.second();
                g_Engine.m_SceneManager->SetActiveScene(newScene);
            }
        }
    }
} 