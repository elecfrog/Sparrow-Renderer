/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once

namespace Sparrow
{
    class RenderManager
    {
    public:
        RenderManager() = default;
        ~RenderManager();
        void Initialize();
    public:
        class PipelineLayoutManager* m_PipelineLayoutManager;
        class ShaderManager* m_ShaderManager { nullptr };
    };
} // Sparrow
