#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "Platform/WindowSystem/WindowSystem.h"

class Scene
{
public:
	Scene(WindowSystem* windowSystem)
		: m_WindowSystem(windowSystem)
	{ }

	virtual ~Scene() {}

	virtual void InitOpenGLFunctions() {}
	virtual void OnInput() {}
	virtual void OnUpdate(float deltaTime = 0.0f) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}

	const std::vector<std::shared_ptr<elf::GameObject>>& GetGameObjects() const
	{
		return m_GameObjects;
	}

	// TODO: This function should move to Render Class
	std::function<void(bool)> fn_wireframeMode{ [=](bool render_mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, render_mode ? GL_LINE : GL_FILL);
	} };

protected:
	WindowSystem* m_WindowSystem;
	std::vector<std::shared_ptr<elf::GameObject>> m_GameObjects;

};