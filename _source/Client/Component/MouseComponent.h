#pragma once

namespace Sparrow
{
    struct MousePosition
    {
        double x{}, y{};
    };

    struct MouseComponent
    {
        bool bIsDragging{false};
        MousePosition start{};
        MousePosition curr{};
    };
}
