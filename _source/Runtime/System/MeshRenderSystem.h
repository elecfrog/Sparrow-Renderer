/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "BaseDefinition.h"
#include "Component/MeshRendererComponent.h"
#include "Entity/Entity.h"

namespace Sparrow
{
    class MeshRenderSystem
    {
    public:
        void Tick(Float delta_time);

        void TickComponent(MeshRendererComponent& component);

        void AddComponent(EntityId entity_id, MeshRendererComponent&& component);

        MeshRendererComponent* GetComponent(UInt entity_id)
        {
            return &m_MeshRendererComponents[entity_id];
        }

    private:
        HashMap<EntityId, MeshRendererComponent> m_MeshRendererComponents;
    };
} // Sparrow
