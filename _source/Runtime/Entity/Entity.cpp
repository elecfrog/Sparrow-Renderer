/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Entity.h"

namespace Sparrow
{
    HashMap<EntityId, Entity*> g_EntityMap;

    std::atomic<EntityId> EntityIdGenerator::s_nextId{0};

    Entity::Entity()
    {
        m_EntityId = EntityIdGenerator::GetNextId();
        g_EntityMap[m_EntityId] = this;
    }

    Entity::~Entity()
    {
        g_EntityMap.erase(m_EntityId);
    }
}