/*
 * description:
 * author@elecfrog
 */
#pragma once

#include "Core/Base/BaseDefinition.h"
#include "Component/CameraComponent.h"
#include "Render/Light.h"
#include "Render/MeshRenderer.h"
#include "Render/StaticMesh.h"

namespace Sparrow
{
    struct Plane
    {
        // mesh renderer
        SharedPtr<StaticMesh> meshFilter;
        SharedPtr<MeshRenderer> meshRenderer;

        std::vector<AttribVertex> vertices
        {
            AttribVertex{
                Vector3f(-2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(1.0f, 0.0f)
            }, // Top-left
            AttribVertex{
                Vector3f(-2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(1.0f, 1.0f)
            }, // Bottom-left
            AttribVertex{
                Vector3f(2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(0.0f, 0.0f)
            }, // Top-right
            AttribVertex{
                Vector3f(2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(0.0f, 0.0f)
            }, // Top-right
            AttribVertex{
                Vector3f(-2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(1.0f, 1.0f)
            }, // Bottom-left
            AttribVertex{
                Vector3f(2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
                glm::vec2(0.0f, 1.0f)
            }, // Bottom-right

            //                    AttribVertex{Vector3f(-2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left (swapped)
            //                    AttribVertex{Vector3f(-2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(1.0f, 0.0f)}, // Top-left (swapped)
            //                    AttribVertex{Vector3f(2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(0.0f, 0.0f)}, // Top-right (unchanged)
            //
            //                    AttribVertex{Vector3f(2.0f, 0.0f, 2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(0.0f, 0.0f)}, // Top-right (unchanged)
            //                    AttribVertex{Vector3f(2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(0.0f, 1.0f)}, // Bottom-right (unchanged)
            //                    AttribVertex{Vector3f(-2.0f, 0.0f, -2.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.f),
            //                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left (swapped)

        };

        explicit Plane()
        {
            meshFilter = std::make_shared<StaticMesh>(std::move(StaticMesh{vertices, std::vector<GLuint>{}}));
            meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerVertex, MeshType::TexturedMesh, meshFilter);
        }

        inline void Render(const SharedPtr<Shader>& shader, CameraComponent& camera, glm::mat4& model_matrix, const Light& light)
        {
            shader->Bind();
            meshRenderer->BindVAO();
            //        glm::mat4 I = glm::mat4(1.0f);
            shader->SetUniformMat4f("M", model_matrix);
            shader->SetUniformMat4f("V", camera.viewMatrix);
            shader->SetUniformMat4f("P", camera.projMatrix);

            // Lighting Relevant
            shader->SetUniform3f("light.diffuseColor", light.color);
            shader->SetUniform3f("light.ambientColor", light.ambient_color);
            shader->SetUniform3f("light.position", light.position);
            shader->SetUniform3f("viewPos", camera.cameraPos);

            // Setting Materials
            shader->SetUniform1i("tex_Diffuse", 1);
            shader->SetUniform4f("material.baseColor", glm::vec4(150.0f / 255.0f));

            // DrawCall
            meshRenderer->Render();

            shader->Unbind();
        }
    };
}
