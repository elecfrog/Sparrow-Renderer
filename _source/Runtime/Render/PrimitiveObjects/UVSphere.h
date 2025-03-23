/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "Entity/Entity.h"

namespace Sparrow
{
    class Shader;

    struct MeshComponent;
    struct MeshRendererComponent;

    struct UVSphere : public Entity
    {
        explicit UVSphere(Vector3 center = Vector3(), Float radius = 1.0f);

        void BuildMeshComponent();
        void BuildMeshRendererComponent();

        Vector3 m_Center{};
        Float   m_Radius{1.0f};
        MeshComponent*         m_MeshComponent{nullptr};
        MeshRendererComponent* m_MeshRendererComponent{nullptr};
    };
}
