/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "Entity/Entity.h"

namespace Sparrow
{
    class Shader;
    class MeshComponent;
    class MeshRendererComponent;

    struct Plane : public Entity
    {
        Plane() = default;

    public:
        void BuildMeshComponent();
        void BuildMeshRendererComponent(SharedPtr<Shader> shader);

        MeshComponent* m_MeshComponent;
        MeshRendererComponent* m_MeshRendererComponent;
    };
}
