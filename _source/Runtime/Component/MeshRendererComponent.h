/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "Render/MeshRenderer.h"

namespace Sparrow
{


    struct MeshRendererComponent
    {
        explicit MeshRendererComponent() = default;

        explicit MeshRendererComponent(MeshComponent& mesh_component)
        {
            m_MeshComponent = &mesh_component;
            m_PositionVBO   = std::make_unique<VBO>(m_MeshComponent->m_PositionBuffer);
            m_NormalsVBO    = std::make_unique<VBO>(m_MeshComponent->m_NormalsBuffer);
            m_TexCoord0VBO  = std::make_unique<VBO>(m_MeshComponent->m_TexCoord0Buffer);
            m_EBO           = std::make_unique<EBO>(m_MeshComponent->m_IndicesBuffer);

            m_VAO = std::make_unique<VAO>();
            m_VAO->Bind();
            m_VAO->LinkAttrib(*m_PositionVBO, 0, 3, GL_FLOAT, sizeof(Vector3f), (void*)0);
            m_VAO->LinkAttrib(*m_NormalsVBO, 1, 3, GL_FLOAT, sizeof(Vector3f), (void*)0);
            m_VAO->LinkAttrib(*m_TexCoord0VBO, 2, 2, GL_FLOAT, sizeof(Vector2f), (void*)0);
            m_VAO->Unbind();
        }

    public:
        MeshComponent* m_MeshComponent {        nullptr        };
        RenderMode     m_RenderMode    { RenderMode::PerVertex };
        UniquePtr<VAO> m_VAO;
        UniquePtr<EBO> m_EBO;
        UniquePtr<VBO> m_PositionVBO;
        UniquePtr<VBO> m_NormalsVBO;
        UniquePtr<VBO> m_TexCoord0VBO;
        std::weak_ptr<Shader> m_Shader;
    };
} // Sparrow
