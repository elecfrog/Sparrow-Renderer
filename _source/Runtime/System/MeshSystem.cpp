/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "MeshSystem.h"

namespace Sparrow
{
    void MeshSystem::Tick(Float delta_time)
    {
        for (auto& [entity_id, component] : m_MeshComponents)
        {
            TickComponent(component);
        }
    }
}
