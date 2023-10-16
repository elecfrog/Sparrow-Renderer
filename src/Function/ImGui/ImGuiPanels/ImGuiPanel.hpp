#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <spdlog/spdlog.h>
#include "Function/ImGui/ImGuiDefinitions.h"

namespace elf
{
	class ImGuiPanel
	{
	public:
		ImGuiPanel(std::string title, bool* open = nullptr)
			: m_title(std::move(title))
			, m_open(open)
		{ }

		virtual ~ImGuiPanel() = default;

		void Render()
		{
			Begin();
			Draw();
			End();
		}

	protected:
		virtual void Begin()
		{
			ImGui::Begin(m_title.c_str(), m_open);
		}

		virtual void Draw() = 0;

		virtual void End()
		{
			ImGui::End();
		}

	private:
		std::string m_title;
		bool* m_open;
	};

}