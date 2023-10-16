#pragma once

#include "Function/ImGui/ImGuiDefinitions.h"

namespace elf
{
	class ImGuiDockSpace
	{
	public:
		ImGuiDockSpace() = default;

		static void Render(const std::string& name)
		{
			/*
				ImGui::SetNextWindowPos(m_position);
				ImGui::SetNextWindowSize(m_size);
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoDocking;
			*/

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			// ImGui::SetNextWindowPos(viewport->Pos);
			// ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

			ImGui::Begin(name.c_str(), nullptr, window_flags);
			ImGui::PopStyleVar(3);

			const ImGuiID dockspace_id = ImGui::GetID(name.c_str());
			ImGui::DockSpace(dockspace_id);

			ImGui::End();
		}
	};
}