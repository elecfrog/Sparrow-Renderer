#pragma once

#include "RHI/OpenGL.h"
#include "RHI/IndexBuffer.h"
#include "Mesh.h"
#include "Component/MeshComponent.h"
#include "Render/StaticMesh.h"

namespace Sparrow
{
    enum class DepthComp
    {
        EQUAL,
        LEQUAL,
        LESS
    };

    enum class PostProcessingEffects
    {
        None = 0,
        Gauss,
        FXAA
    };

    enum class RenderAPIType
    {
        None = 0,
        OpenGL,
    };

    enum class RenderMode
    {
        PerVertex,
        PerTriangle,
        PerTriangle_Strip
    };

    enum class MeshType
    {
        Unknown,
        TexturedMesh,
        SkinnedMesh,
        Count
    };

    class MeshRenderer
    {
    public:
        explicit MeshRenderer(RenderMode renderMode, MeshType meshType, const std::shared_ptr<StaticMesh>& mesh)
            : mesh(mesh), mode(renderMode)
        {
            // Init Objects
            m_VAO = std::make_shared<VAO>();
            m_VAO->Bind();
            vbo = std::make_shared<VBO>(mesh->vertices);
            if (renderMode != RenderMode::PerVertex)
            {
                m_EBO = std::make_shared<EBO>(mesh->indices);
            }

            if (meshType == MeshType::TexturedMesh)
            {
                // Linking Vertex Attributes
                m_VAO->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(AttribVertex),
                                  ((void*)offsetof(AttribVertex, position)));
                m_VAO->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, normal)));
                m_VAO->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, tangent)));
                m_VAO->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(AttribVertex),
                                  ((void*)offsetof(AttribVertex, texCoords)));
            }

            if (meshType == MeshType::TexturedMesh)
            {
                m_VAO->Unbind();
                vbo->Unbind();
                m_EBO->Unbind();
            }
        }

        explicit MeshRenderer(const MeshComponent& mesh_component)
        {
            m_MeshComponent = mesh_component;

            // Init Objects
            m_PositionVBO = std::make_unique<VBO>(m_MeshComponent.m_PositionBuffer);
            m_NormalsVBO  = std::make_unique<VBO>(m_MeshComponent.m_NormalsBuffer);
            m_TexCoord0VBO = std::make_unique<VBO>(m_MeshComponent.m_TexCoord0Buffer);
            // m_EBO          = std::make_shared<EBO>(indices);

            m_VAO = std::make_shared<VAO>();
            m_VAO->Bind();

            // 绑定 position VBO
            m_VAO->LinkAttrib(*m_PositionVBO, 0, 3, GL_FLOAT, sizeof(Vector3f), (void*)0);
            // 绑定 normal VBO
            m_VAO->LinkAttrib(*m_NormalsVBO, 1, 3, GL_FLOAT, sizeof(Vector3f), (void*)0);
            // // 绑定 tangent VBO
            // m_VAO->LinkAttrib(*m_TexCoord0VBO, 2, 3, GL_FLOAT, sizeof(Vector3f), (void*)0);
            // 绑定 texCoord VBO
            m_VAO->LinkAttrib(*m_TexCoord0VBO, 2, 2, GL_FLOAT, sizeof(Vector2f), (void*)0);
            m_VAO->Unbind();
            // 绑定 EBO
            m_EBO = std::make_shared<EBO>(m_MeshComponent.m_IndicesBuffer);

            m_VAO->Unbind();
            vbo->Unbind();
            m_EBO->Unbind();
        }

        void BindVAO() const
        {
            m_VAO->Bind();
        }

        void Clear()
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        }

        void Render(RenderMode renderMode)
        {
            BindVAO();
            if (renderMode == RenderMode::PerTriangle)
            {
                GLCall(
                    glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *)
                        nullptr));
            }
            if (renderMode == RenderMode::PerTriangle_Strip)
            {
                GLCall(
                    glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const
                        void *) nullptr));
            }
            if (renderMode == RenderMode::PerVertex)
            {
                //            glDraw
                GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()));
            }
        }

        void Render()
        {
            BindVAO();
            if (mode == RenderMode::PerTriangle)
            {
                GLCall(
                    glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *)
                        nullptr));
            }
            if (mode == RenderMode::PerTriangle_Strip)
            {
                GLCall(
                    glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const
                        void *) nullptr));
            }
            if (mode == RenderMode::PerVertex)
            {
                //            glDraw
                GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()));
            }
        }

        void RenderEx()
        {
            BindVAO();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, m_MeshComponent.m_VertexCount));
        }

    public:
        std::unique_ptr<VBO> m_PositionVBO;
        std::unique_ptr<VBO> m_NormalsVBO;
        std::unique_ptr<VBO> m_TexCoord0VBO;
        MeshComponent m_MeshComponent {} ;


    public:
        SharedPtr<StaticMesh> mesh;
        SharedPtr<VAO> m_VAO;
        SharedPtr<VBO> vbo;
        SharedPtr<EBO> m_EBO;

        RenderMode mode;
    };
}
