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

    // https://community.khronos.org/t/using-vbos-to-draw-a-cylinder-with-selectable-faces/107232/9
    struct Cylinder : public Entity
    {
        Cylinder() = default;
        explicit Cylinder(Vector3f start, Vector3f end);

        void BuildMeshComponent();
        void BuildMeshRendererComponent(SharedPtr<Shader> shader);

    public:
        Vector3f start{};
        Vector3f end{};
        MeshComponent* m_MeshComponent;
        MeshRendererComponent* m_MeshRendererComponent;
    };
}
