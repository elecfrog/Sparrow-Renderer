/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/23.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#include "Scene_LoadModel.hpp"
#include "Render/PipelineLayoutManager.h"
#include "Render/RenderManager.h"
#include "Render/UBO.h"

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

    void Scene_LoadModel::InitRenderPass()
    {
        PipelineLayoutManager*    pipeline_layout_manager = g_Engine.m_RenderManager->m_PipelineLayoutManager;
        SharedPtr<PipelineLayout> layout = pipeline_layout_manager->GetPipelineLayout("DefaultPipeline");

        m_RenderPass.SetShader(m_PlaneShader);
        m_RenderPass.SetPipelineLayout(layout);
        m_RenderPass.SetMainCameraComponent(&mainCamera);
        m_RenderPass.SetLightComponent(&m_Light);
    }
}
