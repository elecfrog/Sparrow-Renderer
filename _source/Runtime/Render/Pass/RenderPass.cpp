/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "RenderPass.h"

#include "Component/CameraComponent.h"
#include "Component/MeshRendererComponent.h"
#include "Engine/Engine.h"
#include "Entity/Entity.h"
#include "Render/Light.h"
#include "Render/PipelineLayout.h"
#include "Render/UBO.h"
#include "Render/Shader/ShaderManager.h"
#include "Render/RHI/OpenGL.h"
#include "System/MeshRenderSystem.h"

namespace Sparrow
{
    void RenderPass::Execute()
    {
        BeginRender();

        for (EntityId entityId : m_RenderEntityIds)
        {
            auto* mesh_renderer_component = g_Engine.m_MeshRenderSystem->GetComponent(entityId);
            if (!mesh_renderer_component)
            {
                continue;
            }

            auto* mesh_component = mesh_renderer_component->m_MeshComponent;
            if (!mesh_component)
            {
                continue;
            }

            UpdateResources(entityId);
            mesh_renderer_component->m_VAO->Bind();
            switch (mesh_renderer_component->m_RenderMode)
            {
            case RenderMode::PerTriangle:
                {
                    mesh_renderer_component->m_EBO->Bind();
                    UInt index_count = mesh_component->m_IndexCount;
                    GLCall(glDrawElements(GL_TRIANGLES, index_count * sizeof(GLuint), GL_UNSIGNED_INT, 0));
                    mesh_renderer_component->m_EBO->Unbind();
                    break;
                }
            case RenderMode::PerTriangle_Strip:
                {
                    mesh_renderer_component->m_EBO->Bind();
                    UInt index_count = mesh_component->m_IndexCount;
                    GLCall(glDrawElements(GL_TRIANGLE_STRIP, index_count * sizeof(GLuint), GL_UNSIGNED_INT, 0));
                    mesh_renderer_component->m_EBO->Unbind();
                    break;
                }
            case RenderMode::PerVertex:
                GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh_component->m_VertexCount));
                break;
            }
            mesh_renderer_component->m_VAO->Unbind();
        }
        m_RenderEntityIds.clear();

        EndRender();
    }

    void RenderPass::UpdateResources(EntityId entity_id)
    {
        if (!m_PipelineLayout)
        {
            return;
        }

        if (g_EntityMap.find(entity_id) == g_EntityMap.end())
        {
            return;
        }
        Entity* entity = g_EntityMap[entity_id];

        // 更新Matrices UBO
        MatricesUBO matrices;
        matrices.model = entity->m_TransformComponent.GetTransformMatrix();
        matrices.view = m_CameraComponent->viewMatrix;
        matrices.projection = m_CameraComponent->projMatrix;
        m_PipelineLayout->UpdateUBO("Matrices", &matrices);

        // 更新Camera UBO
        CameraUBO cameraData;
        cameraData.cameraPos = m_CameraComponent->cameraPos;
        m_PipelineLayout->UpdateUBO("Camera", &cameraData);

        // 更新Light UBO
        LightUBO lightData;
        lightData.position = m_LightComponent->position;
        lightData.color = m_LightComponent->color;
        lightData.ambient = m_LightComponent->ambient_color;
        m_PipelineLayout->UpdateUBO("Light", &lightData);

        // Setting Materials
        m_Shader->SetUniform1i("tex_Diffuse", 1);
        m_Shader->SetUniform4f("material.baseColor", glm::vec4(150.0f / 255.0f));
    }

    void RenderPass::EnqueueRenderEntity(EntityId entity_id)
    {
        auto* mesh_renderer_component = g_Engine.m_MeshRenderSystem->GetComponent(entity_id);
        if (!mesh_renderer_component)
        {
            return;
        }
        m_RenderEntityIds.push_back(entity_id);
    }

    void RenderPass::BeginRender() const
    {
        m_Shader->Bind();
    }

    void RenderPass::EndRender() const
    {
        m_Shader->Unbind();
    }
}
