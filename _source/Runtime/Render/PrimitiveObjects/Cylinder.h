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

    // https://community.khronos.org/t/using-vbos-to-draw-a-cylinder-with-selectable-faces/107232/9
    struct Cylinder : public Entity
    {
        Cylinder() = default;
        explicit Cylinder(Vector3 top_center, Vector3 bottom_center);

    private:
        void BuildMeshComponent();
        void BuildMeshRendererComponent();

    public:
        Vector3 m_TopCenter{};
        Vector3 m_BottomCenter{};
        MeshComponent* m_MeshComponent{nullptr};
        MeshRendererComponent* m_MeshRendererComponent{nullptr};
    };
}
