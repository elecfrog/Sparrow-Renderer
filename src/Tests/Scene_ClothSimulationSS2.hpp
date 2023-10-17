/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <spdlog/spdlog.h>
#include <random>
#include <functional>
#include "ScenePreCompiled.h"
#include "Function/Render/TextureCube.h"
#include "Core/Maths/Maths.h"

class Scene_ClothSimulationSS2 : public Scene {

    // plane
    Plane planeObject;
    std::shared_ptr<Shader> planeShader;

    // sphere
    UVSphere sphereObject;
    std::shared_ptr<Shader> sphereShader;

    //cloth
    std::shared_ptr<Cloth> clothObject;
    std::shared_ptr<MeshRenderer> clothMeshRenderer;
    std::shared_ptr<Shader> clothShader;

    // GUI Variables
    bool is_wireframe = false;
    bool reloadShaders = false;
    bool renderSphere = true;
    bool cloth_simulation = false;

    // skybox Members
    unsigned int skybox_VAO, skybox_VBO;
    std::shared_ptr<Shader> skybox_Shader;
    std::shared_ptr<TextureCube> skybox_textureCube;

	//wind
    float windDirectionX = 0;
    float windDirectionY = 0;
    float windDirectionZ = 0;

    bool corner_fixed = true;

public:
    Scene_ClothSimulationSS2(WindowSystem *windowSystem)
            : Scene(windowSystem) {
        InitOpenGLFunctions();
        Scene::RegisterInputs();

        {
            // Init Skybox
            GLCall(glGenVertexArrays(1, &skybox_VAO));
            GLCall(glGenBuffers(1, &skybox_VBO));
            GLCall(glBindVertexArray(skybox_VAO));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr)));
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            GLCall(glBindVertexArray(0));
        }

        skybox_Shader = std::make_shared<Shader>(ShaderPath("skybox/cubemap.vert"), ShaderPath("skybox/cubemap.frag"));
        skybox_Shader->Bind();
        skybox_textureCube = std::make_shared<TextureCube>(texCube_Lake);
        skybox_textureCube->Bind(4);

        planeShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane.frag"));
        sphereShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane.frag"));
        clothShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane.frag"));

        auto data = ImportOBJ(AssetPath("cloth/cloth.obj"));
        clothObject = std::make_shared<Cloth>(glm::vec3(0), data);

    }

    ~Scene_ClothSimulationSS2() override = default;

    void InitOpenGLFunctions() override {
        //glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
    }

    void OnUpdate(float _deltaTime = 1.0f / 24.0f) override {
        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();
        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        auto deltaTime = float(currentTime - lastTime);
        if (deltaTime > 1.0f / 60.0f) {
            Scene::ProcessInput(deltaTime);
            mainCamera.UpdateCameraMatrix();
            fn_wireframeMode(is_wireframe);
        }

        if (deltaTime > _deltaTime) {
            if (cloth_simulation) {
                if (!corner_fixed) {
                    clothObject->UpdateFixedPoint(3);
                    clothObject->UpdateFixedPoint(4);
                }

                clothObject->ComputeGravity();
                clothObject->ComputeSpringsForce(1.0f / 24.0f);
                clothObject->ComputeNodesForce(1.0f / 24.0f / 4.0f);
                clothObject->ComputeAirForce();
                clothObject->ComputeWindForce(glm::vec3(windDirectionX, windDirectionY, windDirectionZ));

                clothObject->CollisionGround(-19.8);

                if (renderSphere)
                    clothObject->CollisionSphere(glm::vec3(0.f, -18.0f, 0.f), 2.1f);
                
                clothObject->UpdateNormal();
            }
        }
    }

    void OnRender() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (reloadShaders) {
            ReloadShader(planeShader);
            ReloadShader(sphereShader);
            reloadShaders = false;
            SPW_INFO("Reload Shaders");
        }

        glEnable(GL_DEPTH_TEST);

        glm::mat4 I = glm::mat4(1.0f);
        if (renderSphere) {
            glm::mat4 S = glm::scale(I, glm::vec3(0.1));
            glm::mat4 M = glm::translate(I, glm::vec3(0.f, -0.9f, 0.f)) * S;
            sphereObject.Render(sphereShader, mainCamera, M, light);
        }

        {
            /* Draw plane */
            glm::mat4 M = glm::translate(I, glm::vec3(0.0f, -1.0f, 0.0f));
            planeObject.Render(planeShader, mainCamera, M, light);
        }

        {
            // Skybox
            // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            skybox_Shader->Bind();

            glm::mat3 v_mat3 = glm::mat3(mainCamera.viewMatrix);
            glm::mat4 v_mat4 = glm::mat4(v_mat3);
            skybox_Shader->SetUniformMat4f("V", v_mat4);
            skybox_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

            glBindVertexArray(skybox_VAO);
            glActiveTexture(GL_TEXTURE4);
            skybox_Shader->SetUniform1i("skybox", 4);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            // Switch back to the normal depth function
            glDepthFunc(GL_LESS);
        }

        //render cloth
        if (clothObject != nullptr) {
            clothShader->Bind();
            glm::mat4 I = glm::mat4(1.0f);
            glm::mat4 clothModelMatrix = I;
            clothModelMatrix *= glm::scale(I, glm::vec3(0.05, 0.05, 0.05));

            clothShader->SetUniformMat4f("M", clothModelMatrix);
            clothShader->SetUniformMat4f("V", mainCamera.viewMatrix);
            clothShader->SetUniformMat4f("P", mainCamera.projMatrix);

            // Lighting Relevant
            clothShader->SetUniform3f("light.diffuseColor", light.color);
            clothShader->SetUniform3f("light.ambientColor", light.ambient_color);
            clothShader->SetUniform3f("light.position", light.position);
            clothShader->SetUniform3f("viewPos", mainCamera.cameraPos);

            // Setting Materials
            clothShader->SetUniform1i("tex_Diffuse", 1);
            clothShader->SetUniform4f("material.baseColor",
                                      glm::vec4(157.0f / 255.0f, 230.0f / 255.0f, 110.0f / 255.0f, 1.0f));

            std::shared_ptr<StaticMesh> mesh = std::make_shared<StaticMesh>();
            mesh->vertices = clothObject->GetTriangles();

            clothMeshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerVertex, MeshType::TexturedMesh, mesh);
            clothMeshRenderer->Render();

            clothShader->Unbind();
        }


    }

    void OnImGuiRender() override {
        ImGui::Checkbox("Wireframe Mode", &is_wireframe);
        if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F')) reloadShaders = true;

        ImGui::Separator();

        ImGui::BeginChild("SS2");

        if (ImGui::Button("Export OBJ File")) {
            auto path = FileSystem::NativeFileDialog();
            ExportOBJ(path, clothObject);
        }

        ImGui::SameLine();
        if (ImGui::Button("Import OBJ File")) {
            cloth_simulation = false;
            auto path = FileSystem::NativeFileDialog();

            auto data = ImportOBJ(path);
            clothObject.reset();
            clothObject = std::make_shared<Cloth>(glm::vec3(0), data);
        };


        ImGui::Separator();
        if (ImGui::Button("Start Cloth Simulation")) {
            cloth_simulation = true;
            corner_fixed = false;
        }

        if (ImGui::Button("Reset")) {
            cloth_simulation = false;
            clothObject.reset();
            clothObject = std::make_shared<Cloth>(glm::vec3(0), 16, 16, 2);
        };

        ImGui::Checkbox("Fix the Corner", &corner_fixed);
        ImGui::Checkbox("Render Sphere", &renderSphere);

        ImGui::Text("Wind Directions");
        ImGui::SliderFloat("Direction X", &windDirectionX, -10.0f, 10.0f);
        ImGui::SliderFloat("Direction Y", &windDirectionY, -10.0f, 10.0f);
        ImGui::SliderFloat("Direction Z", &windDirectionZ, -10.0f, 10.0f);

        ImGui::EndChild();
    }
};
