/*
 * description:
 * author@elecfrog
 */
#pragma once

#include "Tests/ScenePreCompiled.h"
#include "Resource/StaticMesh.h"
#include "Function/Render/MeshRenderer.h"

struct UVSphere {
    // mesh renderer
    std::shared_ptr<StaticMesh> meshFilter;
    std::shared_ptr<MeshRenderer> meshRenderer;

    std::vector<AttribVertex> vertices;


    explicit UVSphere(glm::vec3 origin = glm::vec3()) {
        std::vector<GLuint> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                float xSegment = (float) x / (float) X_SEGMENTS;
                float ySegment = (float) y / (float) Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                vertices.emplace_back(
                        AttribVertex{
                                glm::vec3(xPos, yPos, zPos) + origin,
                                glm::vec3(xPos, yPos, zPos),
                                glm::vec3(0.0f),
                                glm::vec2(xSegment, ySegment)}
                );
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        meshFilter = std::make_shared<StaticMesh>(std::move(StaticMesh{vertices, indices}));
        meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerTriangle_Strip, MeshType::TexturedMesh,
                                                      meshFilter);
    }

    inline void
    Render(const std::shared_ptr<Shader> &shader, Camera &camera, glm::mat4 &model_matrix, const Light &light) {

        shader->Bind();

        shader->SetUniformMat4f("M", model_matrix);
        shader->SetUniformMat4f("V", camera.viewMatrix);
        shader->SetUniformMat4f("P", camera.projMatrix);
        shader->SetUniform3f("light.diffuseColor", light.color);
        shader->SetUniform3f("light.ambientColor", light.ambient_color);
        shader->SetUniform3f("light.position", light.position);
        shader->SetUniform3f("viewPos", camera.cameraPos);

        // Setting Materials
        shader->SetUniform1i("tex_Diffuse", 1);
        shader->SetUniform4f("material.baseColor", glm::vec4(243.0f / 255.0f, 200.0f / 255.0f, 203.0f / 255.0f, 1.0f));

        // DrawCall
        meshRenderer->Render();

        shader->Unbind();
    }
};