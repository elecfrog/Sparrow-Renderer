/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/9.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "Base/BaseDefinition.h"
#include "Client/ImGui/ImGuiPanels/ImGuiPanel.hpp"

namespace Sparrow
{
    class MenuPanel : public ImGuiPanel
    {
    public:
        explicit MenuPanel(const std::string& title = "Scenes", bool* open = nullptr)
            : ImGuiPanel(title, open)
        {
        }

    protected:
        void Draw() override;
    };
}

