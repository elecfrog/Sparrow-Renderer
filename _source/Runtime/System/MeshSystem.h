/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "BaseDefinition.h"
#include "Component/MeshComponent.h"
#include "Entity/EntityIdGenerator.h"

namespace Sparrow
{
    class MeshSystem
    {
    public:
        void Tick(Float delta_time);

        void TickComponent(MeshComponent& component)
        {
        }

        void AddComponent(EntityId entity_id, MeshComponent&& component)
        {
            m_MeshComponents.emplace(entity_id, std::move(component));
        }

        MeshComponent* GetComponent(EntityId entity_id)
        {
            return &m_MeshComponents[entity_id];
        }

    private:
        HashMap<EntityId, MeshComponent> m_MeshComponents;
    };
} // Sparrow
