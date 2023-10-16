#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "Platform/WindowSystem/WindowSystem.h"
#include "Scene.hpp"

class SceneManager
{
public:
	SceneManager(WindowSystem* windowSystem)
		: m_WindowSystem(windowSystem)
	{	}

	std::shared_ptr<Scene> GetActiveScene() 
	{ 
		return m_ActiveScene; 
	}
	
	void SetActiveScene(const std::shared_ptr<Scene>& scene) 
	{ 
		m_ActiveScene = scene; 
	}

	template <class T>
	void RegisterScene(const std::string& name)
	{
		LogSystem::Info("Registering Scene  - {0}", name);
		m_Scenes.emplace_back(std::pair{name, [this]() { return std::make_shared<T>(m_WindowSystem); }});
	}

	auto GetScenes() const
	{
		return m_Scenes;
	}

	std::shared_ptr<Scene> GetScene(const std::string& name)
	{
		auto it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [&name](const auto& entry) { return entry.first == name; });
		if (it != m_Scenes.end())
		{
			return it->second();
		}
		return nullptr;
	}

private:
	WindowSystem* m_WindowSystem;
	std::vector<std::pair<std::string, std::function<std::shared_ptr<Scene>()>>> m_Scenes;
	// std::unordered_map<std::string, std::function<Scene*>> m_SceneMap;
	// std::vector<unique_ptr<Scene>> scenes;
	std::shared_ptr<Scene> m_ActiveScene;
	// Scene* m_ActiveScene;
};
