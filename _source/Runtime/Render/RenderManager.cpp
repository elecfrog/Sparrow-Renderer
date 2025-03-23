/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "RenderManager.h"

#include "PipelineLayoutManager.h"
#include "Shader/ShaderManager.h"

namespace Sparrow
{
    RenderManager::~RenderManager()
    {
        delete m_ShaderManager;
        delete m_PipelineLayoutManager;
    }

    void RenderManager::Initialize()
    {
        m_PipelineLayoutManager = new PipelineLayoutManager();
        m_ShaderManager         = new ShaderManager();
    }
} // Sparrow
