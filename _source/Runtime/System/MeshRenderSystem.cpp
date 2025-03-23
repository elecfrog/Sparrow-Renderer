/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "MeshRenderSystem.h"

namespace Sparrow
{
    void MeshRenderSystem::Tick(Float delta_time)
    {
        for (auto& [entity_id, component] : m_MeshRendererComponents)
        {
            TickComponent(component);
        }
    }

    void MeshRenderSystem::TickComponent(MeshRendererComponent& component)
    {
        return;
    }

    void MeshRenderSystem::AddComponent(EntityId entity_id, MeshRendererComponent&& component)
    {
        m_MeshRendererComponents.emplace(entity_id, std::move(component));
    }
} // Sparrow