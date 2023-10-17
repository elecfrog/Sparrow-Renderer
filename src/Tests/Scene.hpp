#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Function/Controller/Camera.h"
#include "Function/Controller/MouseCursor.h"
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
	virtual void RegisterInputs()
	{
		m_WindowSystem->RegisterOnMouseScrollFunc(std::bind(&Scene::OnMouseScrolling, this, std::placeholders::_1, std::placeholders::_2));
		m_WindowSystem->RegisterOnMouseButtonFunc(std::bind(&Scene::onMouseButtonClicked, this, std::placeholders::_1, std::placeholders::_2));
		m_WindowSystem->RegisterOnCursorPosFunc(std::bind(&Scene::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
	}

	const std::vector<std::shared_ptr<elf::GameObject>>& GetGameObjects() const
	{
		return m_GameObjects;
	}

	// TODO: This function should move to Render Class
	std::function<void(bool)> fn_wireframeMode{ [=](bool render_mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, render_mode ? GL_LINE : GL_FILL);
	} };


	inline void OnMouseScrolling(double xoffset, double yoffset)
	{
		this->mainCamera.cameraPos -= static_cast<float>(yoffset) * 0.5f;
	}

	inline void onMouseButtonClicked(int button, int action)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			m_MouseCursor.bIsDragging = true;
			glfwGetCursorPos(m_WindowSystem->GetWindowHandle(), &m_MouseCursor.start.x, &m_MouseCursor.start.y);
			// SPW_INFO("Drag Pressed : {0}, {1}", start_x, start_y);
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			m_MouseCursor.bIsDragging = false;
			SPW_INFO("Drag End");
		}
	}

	inline void onCursorPos(double xpos, double ypos)
	{
		if (m_MouseCursor.bIsDragging)
		{
			m_MouseCursor.curr.x = xpos;
			m_MouseCursor.curr.y = ypos;
			double dx = m_MouseCursor.curr.x - m_MouseCursor.start.x;
			double dy = m_MouseCursor.curr.y - m_MouseCursor.start.y;

			auto y_offset = dy;
			auto x_offset = dx;
			if (std::fabs(dx) > std::fabs(dy))
			{
				y_offset *= this->mainCamera.keySensitivity;
			}
			else
			{
				x_offset *= this->mainCamera.keySensitivity;
			}
			this->mainCamera.yaw -= x_offset * this->mainCamera.keySensitivity;
			this->mainCamera.pitch -= y_offset * this->mainCamera.keySensitivity;

			m_MouseCursor.start.x = m_MouseCursor.curr.x;
			m_MouseCursor.start.y = m_MouseCursor.curr.y;
		}
	}

	virtual void ProcessInput(float deltaTime)
	{
		GLFWwindow* window = m_WindowSystem->GetWindowHandle();
		float cameraSpeed = 0.10f * deltaTime; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			mainCamera.cameraPos += mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			mainCamera.cameraPos -= mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(
				glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(
				glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
			mainCamera.yaw -= mainCamera.keySensitivity * 5.0f;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
			mainCamera.yaw += mainCamera.keySensitivity * 5.0f;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			mainCamera.pitch -= mainCamera.keySensitivity * 5.0f;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			mainCamera.pitch += mainCamera.keySensitivity * 5.0f;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			mainCamera.FOV -= mainCamera.keySensitivity * 2.0f;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			mainCamera.FOV += mainCamera.keySensitivity * 2.0f;
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			mainCamera.FOV *= -1.0f;
	}

protected:

	// bool is_dragging = false;
	// double start_x = 0, start_y = 0;
	// double curr_x = 0, curr_y = 0;

	MouseCursor m_MouseCursor;
	WindowSystem* m_WindowSystem;
	Camera mainCamera{ glm::vec3(1.45, 0.348, 2.021), 240.f, -11.45f, 45.0f, 0.01f };
	std::vector<std::shared_ptr<elf::GameObject>> m_GameObjects;

};