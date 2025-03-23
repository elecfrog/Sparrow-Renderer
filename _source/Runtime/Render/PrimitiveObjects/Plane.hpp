/*
 * description:
 * author@elecfrog
 */
#pragma once

#include "Component/CameraComponent.h"
#include "Engine/Engine.h"
#include "Entity/Entity.h"
#include "Render/Light.h"
#include "Render/MeshRenderer.h"
#include "System/MeshRenderSystem.h"
#include "Render/UBOManager.h"
#include "Render/PipelineLayoutManager.h"
#include "Render/RenderManager.h"

namespace Sparrow
{
    struct Plane : public Entity
    {
        Plane() = default;

        void PreRender(const SharedPtr<Shader>& shader, CameraComponent& camera, const Light& light)
        {
            shader->Bind();

            // 获取或创建PipelineLayout
            PipelineLayoutManager*    pipeline_layout_manager = g_Engine.m_RenderManager->m_PipelineLayoutManager;
            SharedPtr<PipelineLayout> layout = pipeline_layout_manager->GetPipelineLayout("DefaultPipeline");

            // 更新Matrices UBO
            MatricesUBO matrices;
            matrices.model = m_TransformComponent.GetTransformMatrix();
            matrices.view = camera.viewMatrix;
            matrices.projection = camera.projMatrix;
            layout->UpdateUBO("Matrices", &matrices);

            // 更新Camera UBO
            CameraUBO cameraData;
            cameraData.cameraPos = camera.cameraPos;
            layout->UpdateUBO("Camera", &cameraData);

            // 更新Light UBO
            LightUBO lightData;
            lightData.position = light.position;
            lightData.color = light.color;
            lightData.ambient = light.ambient_color;
            layout->UpdateUBO("Light", &lightData);

            // Setting Materials
            shader->SetUniform1i("tex_Diffuse", 1);
            shader->SetUniform4f("material.baseColor", glm::vec4(150.0f / 255.0f));

            shader->Unbind();
        }

    public:
        void BuildMeshComponent();
        void BuildMeshRendererComponent(SharedPtr<Shader> shader);

        MeshComponent          *m_MeshComponent;
        MeshRendererComponent  *m_MeshRendererComponent;
    };
}
