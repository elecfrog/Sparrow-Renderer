/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <Core/Core.h>
#include "Component/CameraComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Render/Light.h"
#include "Render/MeshRenderer.h"
#include "System/MeshRenderSystem.h"

namespace Sparrow
{
    struct Plane
    {
        explicit Plane()
        {
            BuildMeshComponent();
        }

        void PreRender(const SharedPtr<Shader>& shader, CameraComponent& camera, glm::mat4& model_matrix, const Light& light)
        {
            shader->Bind();
            // m_MeshRendererComponent.m_VAO->Bind();
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
            // m_MeshRendererComponent.Render();

            // shader->Unbind();
        }

    public:
        void BuildMeshComponent()
        {
            m_MeshComponent.m_PositionBuffer  = {
                Vector3f(-2.0f, 0.0f, 2.0f),   // 第 0 个顶点
                Vector3f(-2.0f, 0.0f, -2.0f),  // 第 1 个顶点
                Vector3f(2.0f, 0.0f, 2.0f),    // 第 2 个顶点
                Vector3f(2.0f, 0.0f, 2.0f),    // 第 3 个顶点（重复）
                Vector3f(-2.0f, 0.0f, -2.0f),  // 第 4 个顶点（重复）
                Vector3f(2.0f, 0.0f, -2.0f)    // 第 5 个顶点
            };
            m_MeshComponent.m_NormalsBuffer   = {
                Vector3f(0.0f, 1.0f, 0.0f),    // 第 0 个顶点
                Vector3f(0.0f, 1.0f, 0.0f),    // 第 1 个顶点
                Vector3f(0.0f, 1.0f, 0.0f),    // 第 2 个顶点
                Vector3f(0.0f, 1.0f, 0.0f),    // 第 3 个顶点（重复）
                Vector3f(0.0f, 1.0f, 0.0f),    // 第 4 个顶点（重复）
                Vector3f(0.0f, 1.0f, 0.0f)     // 第 5 个顶点
            };;
            m_MeshComponent.m_TangentsBuffer   = {
                Vector3f(0.f),    // 第 0 个顶点
                Vector3f(0.f),    // 第 1 个顶点
                Vector3f(0.f),    // 第 2 个顶点
                Vector3f(0.f),    // 第 3 个顶点（重复）
                Vector3f(0.f),    // 第 4 个顶点（重复）
                Vector3f(0.f)     // 第 5 个顶点
            };
            m_MeshComponent.m_TexCoord0Buffer  = {
                Vector2f(1.0f, 0.0f),    // 第 0 个顶点
                Vector2f(1.0f, 1.0f),    // 第 1 个顶点
                Vector2f(0.0f, 0.0f),    // 第 2 个顶点
                Vector2f(0.0f, 0.0f),    // 第 3 个顶点（重复）
                Vector2f(1.0f, 1.0f),    // 第 4 个顶点（重复）
                Vector2f(0.0f, 1.0f)     // 第 5 个顶点
            };

            m_MeshComponent.m_IndicesBuffer    = {
                0, 1, 2,   // 第一个三角形
                3, 4, 5    // 第二个三角形
            };

            m_MeshComponent.m_IndexCount  = m_MeshComponent.m_IndicesBuffer.size();
            m_MeshComponent.m_VertexCount = m_MeshComponent.m_PositionBuffer.size();
        }



    public:
        MeshComponent          m_MeshComponent;
        EntityId  id           = 10;


    };
}
