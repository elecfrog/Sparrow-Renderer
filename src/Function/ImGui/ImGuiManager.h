#pragma once

#include "ImGuiDockSpace.h"
#include "Function/ImGui/ImGuiPanels/ImGuiPanel.hpp"
#include "Function/ImGui/ImGuiPanels/ImGuiInspectorPanel.hpp"
#include "Function/ImGui/ImGuiPanels/ImGuiMenuBar.hpp"
#include "Function/ImGui/ImGuiPanels/ImGuiObjectPanel.hpp"
#include "Function/ImGui/ImGuiPanels/ImGuiTreeNodePanel.hpp"
#include "ImGuiDefinitions.h"
#include <memory>

namespace elf
{



	class ImGuiManager
	{
	public:
		ImGuiManager() = default;

		void Init(GLFWwindow* window)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 450");

			InitLayout();
			windowHandle = window;
		}

		void Begin()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void End()
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void CleanUp()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void ShowDemoWindow(bool show_demo_window)
		{
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		void EnableViewport(GLFWwindow* window)
		{
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(window);
			}
		}

		void DisableViewport()
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		template <UIComponentType C>
		void RenderUIComponent() {}

		template <UIComponentType C>
		void RenderUIComponent(const std::string& name){}

		template <UIComponentType C>
		void RenderUIComponent(std::string&& name){}

		std::shared_ptr<ImGuiObjectPanel> GetProfilingPanel() { return m_ObjectPanel; }
		std::shared_ptr<ImGuiInspectorPanel> GetInspectorPanel() { return m_InspectorPanel; }


	private:

		void InitLayout()
		{
			InitMenuBar();
			InitProfilingPanel();
			InitSceneHierarchy();
			InitInspectorPanel();
		}

		void InitMenuBar()
		{
			m_MainMenuBar = std::make_shared<ImGuiMenuBar>("Main Menu Bar");

			m_MainMenuBar->AddSubMenu("File");
			m_MainMenuBar->AddSubMenu("Edit");
			m_MainMenuBar->AddSubMenu("View");
			m_MainMenuBar->AddSubMenu("Tool");
			m_MainMenuBar->AddSubMenu("Help");
			m_MainMenuBar->AddSubMenu("About");
			m_MainMenuBar->AddMenuItemToSubMenu("File", "Import", [&]() { /* ����ļ��˵�������... */ });
			m_MainMenuBar->AddMenuItemToSubMenu("File", "Export", [&]() { /* ��ӱ༭�˵�������... */ });
		}

		void InitProfilingPanel()
		{
			m_ObjectPanel = std::make_shared<ImGuiObjectPanel>("Gmae Object Panel");
		}

		void InitSceneHierarchy()
		{
			m_HierarchyPanel = std::make_shared<ImGuiTreeNodePanel>("Hierarchy Panel");

			m_HierarchyPanel->AddTreeNode("Root", [] { std::cout << "Clicked on Root" << std::endl; });
			m_HierarchyPanel->AddChildTreeNode("Root", "FolderA", [] { std::cout << "Clicked on FolderA" << std::endl; });
			m_HierarchyPanel->AddChildTreeNode("FolderA", "SubfolderA1", [] { std::cout << "Clicked on SubfolderA1" << std::endl; });
			m_HierarchyPanel->AddChildTreeNode("Root", "FolderB", [] { std::cout << "Clicked on FolderB" << std::endl; });
			m_HierarchyPanel->AddChildTreeNode("FolderB", "FileB1", [] { std::cout << "Clicked on FileB1" << std::endl; });
			m_HierarchyPanel->AddChildTreeNode("Root", "File1", [] { std::cout << "Clicked on File1" << std::endl; });
		}

		void InitInspectorPanel()
		{
			m_InspectorPanel = std::make_shared<ImGuiInspectorPanel>("Inspector Panel");
		}

	private:
		std::shared_ptr<ImGuiMenuBar>					m_MainMenuBar;
		std::shared_ptr<ImGuiObjectPanel>				m_ObjectPanel;
		std::shared_ptr<ImGuiTreeNodePanel>				m_HierarchyPanel;
		std::shared_ptr<ImGuiInspectorPanel>			m_InspectorPanel;


		bool show_demo_window = false;
		GLFWwindow* windowHandle = nullptr;

	};

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::Panel>(const std::string& name)
	{
		// ImGuiPanel::Render(name);
	}

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::Dockspace>(std::string&& name)
	{
		ImGuiDockSpace::Render(std::forward<std::string>(name));
	}

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::MenuBar>()
	{
		m_MainMenuBar->Render();
	}

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::ProfilingPanel>()
	{
		m_ObjectPanel->Render();
		// Show a simple window that we create ourselves (use a Begin/End pair to created a named window)
		ImGui::Begin("Demo Window");
		ImGui::Checkbox("Demo Window", &show_demo_window);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		if (show_demo_window)
			ShowDemoWindow(&show_demo_window);
	}

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::HierarchyPanel>()
	{
		m_HierarchyPanel->Render();
	}

	template<>
	void ImGuiManager::RenderUIComponent<UIComponentType::InspectorPanel>()
	{
		m_InspectorPanel->Render();
	}
}