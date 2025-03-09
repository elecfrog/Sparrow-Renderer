/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Component/CameraComponent.h"
#include "Render/Light.h"
#include "Render/MeshRenderer.h"
#include "Render/StaticMesh.h"
#include "Render/RHI/Shader.h"

namespace Sparrow
{
    struct Prototype
    {
        Prototype() = default;

        virtual ~Prototype() = default;

        virtual void Render(Shader& shader, CameraComponent& camera, glm::mat4& matModel, const Light& light) = 0;

        std::shared_ptr<StaticMesh> meshFilter;
        std::shared_ptr<MeshRenderer> meshRenderer;
        std::vector<AttribVertex> vertices;
    };
}
