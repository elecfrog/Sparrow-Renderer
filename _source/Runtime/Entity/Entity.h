/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "Component/TransformComponent.h"

namespace Sparrow
{
    using EntityId = UInt;
    constexpr EntityId k_InvalidEntityId = 0xFFFFFFFF;

    class Entity
    {
    public:
        Entity();
        ~Entity();
    public:
        EntityId           m_EntityId           {k_InvalidEntityId};
        TransformComponent m_TransformComponent {                 };
    };

    extern HashMap<EntityId, Entity*> g_EntityMap;

    class EntityIdGenerator
    {
    public:
        // 获取下一个唯一的 EntityId
        static EntityId GetNextId()
        {
            return s_nextId.fetch_add(1, std::memory_order_relaxed);
        }

        // 重置计数器（如果需要）
        static void Reset(EntityId value = 0)
        {
            s_nextId.store(value, std::memory_order_relaxed);
        }

    private:
        // 使用 atomic 保证线程安全
        static std::atomic<EntityId> s_nextId;
    };
}
