/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "Entity/Entity.h"

namespace Sparrow
{
    class Shader;

    struct MeshComponent;
    struct MeshRendererComponent;

    struct Plane : public Entity
    {
        Plane();

        void BuildMeshComponent();
        void BuildMeshRendererComponent();

        MeshComponent* m_MeshComponent{nullptr};
        MeshRendererComponent* m_MeshRendererComponent{nullptr};
    };
}
