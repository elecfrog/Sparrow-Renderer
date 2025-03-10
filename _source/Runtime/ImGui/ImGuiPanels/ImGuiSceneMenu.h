#pragma once

#include <Core/Core.h>

#include "ImGuiPanel.hpp"

namespace Sparrow
{
    class ImGuiSceneMenu : public ImGuiPanel
    {
    public:
        explicit ImGuiSceneMenu(const std::string& title = "Scenes", Bool* open = nullptr)
            : ImGuiPanel(title, open)
        {
        }

    protected:
        void Draw() override;
    };
} 