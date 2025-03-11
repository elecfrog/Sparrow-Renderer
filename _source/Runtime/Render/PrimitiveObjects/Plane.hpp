/*
 * description:
 * author@elecfrog
 */
#pragma once

#include "Component/CameraComponent.h"
#include "Entity/Entity.h"
#include "Render/Light.h"
#include "Render/MeshRenderer.h"
#include "System/MeshRenderSystem.h"

namespace Sparrow
{
    struct Plane : public Entity
    {
        Plane() = default;

        void PreRender(const SharedPtr<Shader>& shader, CameraComponent& camera, glm::mat4& model_matrix, const Light& light)
        {
            shader->Bind();
            // m_MeshRendererComponent.m_VAO->Bind();
            //        glm::mat4 I = glm::mat4(1.0f);
            shader->SetUniformMat4f("M", model_matrix);
            shader->SetUniformMat4f("V", camera.viewMatrix);
            shader->SetUniformMat4f("P", camera.projMatrix);

            // Lighting Relevant
            shader->SetUniform3f("light.diffuseColor", light.color);
            shader->SetUniform3f("light.ambientColor", light.ambient_color);
            shader->SetUniform3f("light.position", light.position);
            shader->SetUniform3f("viewPos", camera.cameraPos);

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
