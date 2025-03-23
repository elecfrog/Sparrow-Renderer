#pragma once

#include <Core/Core.h>

#include "Entity/Entity.h"

namespace Sparrow
{
    class Shader;
    class PipelineLayout;

    struct Light;
    struct CameraComponent;
    struct MeshRendererComponent;

    class RenderPass
    {
    public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        // virtual void Initialize() = 0;

        void Execute();

        // virtual void Cleanup() = 0;

        void SetShader(const SharedPtr<Shader>& shader) { m_Shader = shader; }
        const SharedPtr<Shader>& GetShader() const { return m_Shader; }

        void SetPipelineLayout(const SharedPtr<PipelineLayout>& pipeline_layout)
        {
            m_PipelineLayout = pipeline_layout;
        }

        void SetMainCameraComponent(CameraComponent* camera)
        {
            m_CameraComponent = camera;
        }

        void SetLightComponent(Light* light)
        {
            m_LightComponent = light;
        }

        void UpdateResources(EntityId entity_id);
        void EnqueueRenderEntity(EntityId entity_id);

    protected:
        void BeginRender() const;
        void EndRender() const;

    protected:
        CameraComponent* m_CameraComponent = nullptr;
        Light*           m_LightComponent = nullptr;
        SharedPtr<Shader>         m_Shader;
        SharedPtr<PipelineLayout> m_PipelineLayout;
        Vector<EntityId>          m_RenderEntityIds;
    };
}
