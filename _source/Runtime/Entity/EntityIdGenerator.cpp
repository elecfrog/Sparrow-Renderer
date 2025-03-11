/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/11.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "EntityIdGenerator.h"

namespace Sparrow
{
    // 初始化静态成员变量
    std::atomic<EntityId> EntityIdGenerator::s_nextId{0};

} // Sparrow
