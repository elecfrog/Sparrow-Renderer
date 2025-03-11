/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "MeshSystem.h"

void Sparrow::MeshSystem::Tick(Float delta_time)
{
    for (auto& [entity_id, component] : m_MeshComponents)
    {
        TickComponent(component);
    }
}
