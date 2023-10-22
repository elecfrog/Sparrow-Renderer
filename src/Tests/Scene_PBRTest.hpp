#pragma once

#include "ScenePreCompiled.h"
#include "Function/Base/Transform.hpp"
#include "Function/Animation/BVHAnim.hpp"
#include "src/Function/Render/Prototype/Cone.hpp"
#include "src/Function/Render/Prototype/Cylinder.hpp"
#include "glm/ext.hpp"


class Scene_PBRTest : public Scene {

    // GUI Variables
    bool is_wireframe = false;
    bool reloadShaders = false;

    // sphere
    UVSphere lightSphere;
    std::shared_ptr<Shader> sphereShader;

    // sphere
    UVSphere PBRSphereObject;
    std::shared_ptr<Shader> PBRSphereShader;

    float roughness = 1.f;

public:
    Scene_PBRTest(WindowSystem *windowSystem)
            : Scene(windowSystem) {
        InitOpenGLFunctions();
        RegisterInputs();

        // Init Plane
        PBRSphereShader = std::make_shared<Shader>(ShaderPath("plane/pbr_base.vert"), ShaderPath("plane/pbr_base.frag"));
        sphereShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane/plane.frag"));
        mainCamera.cameraPos = glm::vec3(1.95, 0.84, 2.52);
    }


    ~Scene_PBRTest() override = default;

    void OnUpdate(float _deltaTime = 0.0f) override {
        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        ProcessInput(deltaTime);

        mainCamera.UpdateCameraMatrix();

        fn_wireframeMode(is_wireframe);
    }

    void OnRender() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        if (reloadShaders) {
            PBRSphereShader = std::make_shared<Shader>(PBRSphereShader->vertPath, PBRSphereShader->fragPath);
            ReloadShader(PBRSphereShader);
            reloadShaders = false;
            SPW_INFO("Reload Shaders");
        }


        { // light sphere
            glm::mat4 M = glm::translate(glm::mat4(1.0f), light.position);
            M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
            lightSphere.Render(sphereShader, mainCamera, M, light);
        }

        PBRSphereShader->Bind();
        PBRSphereShader->SetUniform1f("u_Roughness", roughness);
        PBRSphereShader->Unbind();

        PBRSphereObject.Render(PBRSphereShader, mainCamera, glm::mat4(1.f), light);
    }

    void OnImGuiRender() override {
        ImGui::Checkbox("Wireframe Mode", &is_wireframe);
        if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F')) reloadShaders = true;

        ImGui::SliderFloat("Roughness", &roughness, 0.f, 1.f);
    }
};