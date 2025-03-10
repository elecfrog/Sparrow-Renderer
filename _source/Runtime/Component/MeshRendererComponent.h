/*------------------------------------------------------------------------------------------------------------
* Created by elecfrog on 2025/3/10.
* Copyright (c) 2025 East10 All rights reserved.
------------------------------------------------------------------------------------------------------------ */

#pragma once
#include "Render/MeshRenderer.h"

namespace Sparrow
{
    struct MeshRendererComponentCreationInfo
    {
        MeshComponent* meshComponent {nullptr};
    };

    struct MeshRendererComponent
    {
        explicit MeshRendererComponent() = default;

        explicit MeshRendererComponent(MeshRendererComponentCreationInfo& creationInfo)
        {
            m_MeshComponent = creationInfo.meshComponent;
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

        void Render()
        {
            m_VAO->Bind();
            switch (m_RenderMode)
            {
            case RenderMode::PerTriangle:
                m_EBO->Bind();
                GLCall(glDrawElements(GL_TRIANGLES, m_MeshComponent->m_IndexCount, GL_UNSIGNED_INT, NULL));
                m_EBO->Unbind();
                break;
            case RenderMode::PerTriangle_Strip:
                GLCall(glDrawElements(GL_TRIANGLE_STRIP, m_MeshComponent->m_IndexCount, GL_UNSIGNED_INT, 0));
                break;
            case RenderMode::PerVertex:
                GLCall(glDrawArrays(GL_TRIANGLES, 0, m_MeshComponent->m_VertexCount));
                break;
            }
            m_VAO->Unbind();
        }

    public:
        MeshComponent* m_MeshComponent {        nullptr        };
        RenderMode     m_RenderMode    { RenderMode::PerTriangle };
        UniquePtr<VAO> m_VAO;
        UniquePtr<EBO> m_EBO;
        UniquePtr<VBO> m_PositionVBO;
        UniquePtr<VBO> m_NormalsVBO;
        UniquePtr<VBO> m_TexCoord0VBO;
    };
} // Sparrow
