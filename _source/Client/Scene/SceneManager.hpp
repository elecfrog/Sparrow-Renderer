#pragma once

#include "Scene.hpp"
#include "Scene_LoadModel.hpp"

namespace Sparrow
{

    class SceneManager
    {
    public:
        SceneManager()
        {
            m_Scenes.resize(0);
        }

        SharedPtr<Scene> GetActiveScene()
        {
            return m_ActiveScene;
        }

        void SetActiveScene(const SharedPtr<Scene>& scene)
        {
            m_ActiveScene = scene;
        }

        void RegisterScenes()
        {
            // m_SceneManager->RegisterScene<Scene01_Triangle>("OpenGL Triangle");
            // m_SceneManager->RegisterScene<Scene_BVHViewer>("BVH Animation Viewer");
            RegisterScene<Scene_LoadModel>("Forward+");
            // // m_SceneManager->RegisterScene<Scene_Terrain>("Terrain");
            // m_SceneManager->RegisterScene<Scene06_ShadowMapping>("Shadow Mapping");
            // m_SceneManager->RegisterScene<Scene_ClothSimulationSS1>("Cloth Simulation SS1");
            // m_SceneManager->RegisterScene<Scene_ClothSimulationSS2>("Cloth Simulation SS2");
            // ...
        }

        template <class T>
        void RegisterScene(const std::string& name)
        {
            LOG_INFO(LogModule::App, "Registering Scene  - {}", name);
            m_Scenes.emplace_back(std::pair{
                name, [this]() { return std::make_shared<T>(); }
            });
        }

        auto GetScenes() const
        {
            return m_Scenes;
        }

        SharedPtr<Scene> GetScene(const std::string& name)
        {
            auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(),
                                   [&name](const auto& entry) { return entry.first == name; });
            if (it != m_Scenes.end())
            {
                return it->second();
            }
            return nullptr;
        }

    private:
        std::vector<std::pair<std::string, std::function<SharedPtr<Scene>()>>> m_Scenes;
        SharedPtr<Scene> m_ActiveScene;
    };
}
