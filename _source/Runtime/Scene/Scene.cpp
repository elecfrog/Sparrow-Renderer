/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Scene_LoadModel.hpp"

namespace Sparrow
{
    void Scene_LoadModel::InitSceneRenderPipelineLayouts()
    {
        PipelineLayoutManager* pipeline_layout_manager = g_Engine.m_RenderManager->m_PipelineLayoutManager;

        auto defaultLayout = pipeline_layout_manager->CreatePipelineLayout("DefaultPipeline");

        // 添加UBO描述符
        defaultLayout->AddUBO("Matrices", sizeof(MatricesUBO));
        defaultLayout->AddUBO("Camera", sizeof(CameraUBO));
        defaultLayout->AddUBO("Light", sizeof(LightUBO));

        pipeline_layout_manager->InitAll();
    }
}
