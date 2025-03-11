/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "MeshRenderSystem.h"

namespace Sparrow {
    void MeshRenderSystem::Tick(Float delta_time)
    {
        for (auto& [entity_id, component] : m_MeshRendererComponents)
        {
            TickComponent(component);
        }
    }

    void MeshRenderSystem::TickComponent(MeshRendererComponent& component)
    {
        Render(component);
    }

    void MeshRenderSystem::AddComponent(EntityId entity_id, MeshRendererComponentCreationInfo& creation_info)
    {
        MeshRendererComponent component = MeshRendererComponent(creation_info);
        m_MeshRendererComponents.emplace(entity_id, std::move(component));
    }

    void MeshRenderSystem::AddComponent(EntityId entity_id, MeshRendererComponent&& component)
    {
        m_MeshRendererComponents.emplace(entity_id, std::move(component));
    }

    void MeshRenderSystem::Render(MeshRendererComponent& component)
    {
        auto* mesh_component = component.m_MeshComponent;
        if (!mesh_component)
        {
            return;
        }
        component.m_Shader.lock()->Bind();
        component.m_VAO->Bind();
        switch (component.m_RenderMode)
        {
        case RenderMode::PerTriangle:
            component.m_EBO->Bind();
            GLCall(glDrawElements(GL_TRIANGLES, mesh_component->m_IndexCount * sizeof(GLuint), GL_UNSIGNED_INT, 0));
            component.m_EBO->Unbind();
            break;
        case RenderMode::PerTriangle_Strip:
            GLCall(
                glDrawElements(GL_TRIANGLE_STRIP, mesh_component->m_IndexCount * sizeof(GLuint), GL_UNSIGNED_INT, 0
                ));
            break;
        case RenderMode::PerVertex:
            GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh_component->m_VertexCount));
            break;
        }
        component.m_VAO->Unbind();
        component.m_Shader.lock()->Unbind();
    }
} // Sparrow