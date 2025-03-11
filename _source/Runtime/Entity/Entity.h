/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "EntityId.h"
#include "EntityIdGenerator.h"
#include "Component/TransformComponent.h"

namespace Sparrow
{
    class Entity
    {
    public:
        Entity();
    public:
        EntityId           m_EntityId           {k_InvalidEntityId};
        TransformComponent m_TransformComponent {                 };
    };

    inline Entity::Entity()
    {
        m_EntityId = EntityIdGenerator::GetNextId();
    }
}
