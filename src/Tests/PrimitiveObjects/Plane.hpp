/*
 * description:
 * author@elecfrog
 */
#pragma once

#include "Tests/ScenePreCompiled.h"

struct Plane {
    // mesh renderer
    std::shared_ptr<StaticMesh> meshFilter;
    std::shared_ptr<MeshRenderer> meshRenderer;

    std::vector<TexturedVertex_new> vertices
            {
                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(1.0f, 0.0f)}, // Top-left
                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left
                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(0.0f, 0.0f)},  // Top-right
                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(0.0f, 0.0f)},  // Top-right
                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left
                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
                                       glm::vec2(0.0f, 1.0f)}, // Bottom-right

//                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left (swapped)
//                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(1.0f, 0.0f)}, // Top-left (swapped)
//                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(0.0f, 0.0f)}, // Top-right (unchanged)
//
//                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(0.0f, 0.0f)}, // Top-right (unchanged)
//                    TexturedVertex_new{glm::vec3(2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(0.0f, 1.0f)}, // Bottom-right (unchanged)
//                    TexturedVertex_new{glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.f),
//                                       glm::vec2(1.0f, 1.0f)}, // Bottom-left (swapped)

            };

    explicit Plane() {
        meshFilter = std::make_shared<StaticMesh>(std::move(StaticMesh{vertices, std::vector<GLuint>{}}));
        meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerVertex, MeshType::TexturedMesh, meshFilter);
    }

    inline void Render(const std::shared_ptr<Shader> &shader, Camera &camera, glm::mat4 &model_matrix, const Light &light) {

        shader->Bind();
        meshRenderer->BindVAO();
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

        // DrawCall
        meshRenderer->Render();

        shader->Unbind();
    }
};