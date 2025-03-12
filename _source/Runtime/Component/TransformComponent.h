#pragma once

#include <Core/Core.h>

namespace Sparrow
{
    struct TransformComponent
    {
        Vector3 position {Vector3::Zero()};
        QuaternionW rotation {QuaternionW::Identity()};
        Vector3 scaling  {Vector3::One()};

        Matrix4x4 GetTransformMatrix()
        {
            return Matrix4x4::CreateTRS(position, rotation, scaling);
        }
    };
}
