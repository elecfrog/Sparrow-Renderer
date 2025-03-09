#pragma once

#include <imgui.h>

namespace Sparrow
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