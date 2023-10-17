#pragma once

#include <glad/glad.h>

#include "Core/Utils/Utility.hpp"
#include "RHI/IndexBuffer.h"
#include "RHI/Shader.h"
#include "RHI/VertexArray.hpp"
#include "Mesh.h"
#include "Material.h"
#include "Resource/StaticMesh.h"

enum class DepthComp {
    EQUAL,
    LEQUAL,
    LESS
};

enum class PostProcessingEffects {
    None = 0,
    Gauss,
    FXAA
};

enum class RenderAPIType {
    None = 0,
    OpenGL,
};

enum class RenderMode {
    PerVertex,
    PerTriangle,
    PerTriangle_Strip
};

enum class MeshType {
    Unknown,
    TexturedMesh,
    SkinnedMesh,
    Count
};

class MeshRenderer {
public:
    explicit MeshRenderer(RenderMode renderMode, MeshType meshType, const std::shared_ptr<StaticMesh>& mesh)
            : mesh(mesh),mode(renderMode) {

        // Init Objects
        vao = std::make_shared<elf::VAO>();
        vao->Bind();
        vbo = std::make_shared<elf::VBO>(mesh->vertices);
        if (renderMode != RenderMode::PerVertex) 
        {
            ebo = std::make_shared<EBO>(mesh->indices);
        }

        if (meshType == MeshType::TexturedMesh) {
            // Linking Vertex Attributes
            vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, position)));
            vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, normal)));
            vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, tangent)));
            vao->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(AttribVertex), ((void*)offsetof(AttribVertex, texCoords)));
        }
        
        if (meshType == MeshType::TexturedMesh) {
            vao->Unbind();
            vbo->Unbind();
            ebo->Unbind();
        }
    }

    void BindVAO() const
    {
        vao->Bind();
    }
    
    void Clear() {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void Render(RenderMode renderMode) {
        BindVAO();
        if (renderMode == RenderMode::PerTriangle)
        {
            GLCall(glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *) nullptr));
        }
        if (renderMode == RenderMode::PerTriangle_Strip)
        {
            GLCall(glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *) nullptr));
        }
        if (renderMode == RenderMode::PerVertex)
        {
//            glDraw
            GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()));
        }
    }

    void Render() {
        BindVAO();
        if (mode == RenderMode::PerTriangle)
        {
            GLCall(glDrawElements(GL_TRIANGLES, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *) nullptr));
        }
        if (mode == RenderMode::PerTriangle_Strip)
        {
            GLCall(glDrawElements(GL_TRIANGLE_STRIP, mesh->indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, (const void *) nullptr));
        }
        if (mode == RenderMode::PerVertex)
        {
//            glDraw
            GLCall(glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size()));
        }
    }

public:
    std::shared_ptr<StaticMesh> mesh;
    std::shared_ptr<elf::VAO> vao;
    std::shared_ptr<elf::VBO> vbo;
    std::shared_ptr<EBO> ebo;
    
    RenderMode mode;
};
