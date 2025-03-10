/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

#include "RenderManager.h"

#include "Shader/ShaderManager.h"

namespace Sparrow
{
    RenderManager::~RenderManager()
    {
        delete m_ShaderManager;
    }

    void RenderManager::Initialize()
    {
        m_ShaderManager = new ShaderManager();
    }
} // Sparrow
