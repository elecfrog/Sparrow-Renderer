/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "BaseDefinition.h"
#include "Component/MeshRendererComponent.h"

namespace Sparrow
{
    using EntityId = UInt;

    class MeshRenderSystem
    {
    public:
        void Tick(Float delta_time)
        {
            for (auto& [entity_id, component] : m_MeshRendererComponents)
            {
                TickComponent(component);
            }
        }

        void TickComponent(MeshRendererComponent& component)
        {
            Render(component);
        }

        void AddComponent(MeshRendererComponentCreationInfo& creationInfo)
        {
            MeshRendererComponent component = MeshRendererComponent(creationInfo);
            m_MeshRendererComponents.emplace(10, std::move(component));
        }

    private:
        void Render(MeshRendererComponent& component)
        {
            auto* mesh_component = component.m_MeshComponent;
            if (!mesh_component)
            {
                return;
            }

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
        }

    private:
        HashMap<EntityId, MeshRendererComponent> m_MeshRendererComponents;
    };
} // Sparrow
