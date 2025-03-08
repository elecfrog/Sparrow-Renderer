#pragma once

#include "Base/BaseDefinition.h"

namespace Sparrow
{
    struct Rotation
    {
        Quaternion quaternion{};
        Vector3f euler{};
    };

    struct TransformComponent
    {
        Vector3f position {};
        Rotation rotation {};
        Vector3f scaling  {};

        TransformComponent() : position(Vector3f(0.f)), rotation(Vector3f(0.f)), scaling(Vector3f(1.f))
        {
        }
    };
}
