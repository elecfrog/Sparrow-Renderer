#pragma once

#include "ScenePreCompiled.h"

class Scene08_PointShadow : public Scene
{
    // Model
    std::shared_ptr<Model> cp_cube, sf_cube;

    // Shader
    std::shared_ptr<Shader> scene_Shader;
    std::shared_ptr<Shader> lightView_Shader; // Render in Light View
    std::shared_ptr<Shader> plane_Shader;

    // Camera
    Camera mainCamera{ glm::vec3(1.45, 0.348, 2.021), 240.f, -11.45f, 45.0f, 0.01f };

    // GUI Variables
    bool is_wireframe{ false };
    bool reloadShaders{ false };

    float lightColor[3]{ 1.0f, 1.0f, 1.0f };
    float lightPos[3]{ 0.0f, 0.836f, 2.889f };

    float innerCutOff = 7.5f;
    float outerCutOff = 12.5f;

    float near_plane { 1.f };
    float far_plane  { 7.5f };

    const uint32_t SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    uint32_t depthCubemapFBO, depthCubemap;

    bool visiable_cpCube{ true };
    bool visiable_sfCube{ false };
    bool visiable_plane{ true };
    bool visiable_skybox{ true };
    bool visiable_debugDepthMap{ true };

    // Skybox
    uint32_t skybox_VAO, skybox_VBO;
    std::shared_ptr<Shader> skybox_Shader;
    std::shared_ptr<TextureCube> skybox_textureCube;

    float scaling{ 1.0f };
    float translation[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };

    // Create plane Objects
    uint32_t plane_VAO, plane_VBO, plane_EBO;
    Texture2D* tex_plane_diffuse;

public:
    Scene08_PointShadow(WindowSystem* windowSystem)
            : Scene(windowSystem)
    {
        InitOpenGLFunctions();

        // Load Model Data
        cp_cube = Model::LoadModel(k_ModelList.at("cpCube_GLTF"));
        sf_cube = Model::LoadModel(k_ModelList.at("scifiCube_GLTF"));

        for (const auto& submesh : cp_cube->GetMeshes())
        {
            submesh->BuildMeshFilter();
            submesh->m_Material->GetMaterialProperites().textures.albedo->Bind(0);
        }

        // Load Shader
        scene_Shader = std::make_shared<Shader>(
                "res/Shaders/point_shadow/render.vert",
                "res/Shaders/point_shadow/render.frag");


        // Dealing with subMeshes
        for (const auto& submesh : sf_cube->GetMeshes())
        {
            // Mesh
            submesh->BuildMeshFilter();
            // Material
            // TODO sf_cube has no texture.. so, I need to think about a method to deal with such a case in the future...
            // submesh->m_Material->GetMaterialProperites().textures.albedo->Bind(0);
            // submesh->m_Material->GetAlbedoTexture()->Bind(0);
        }


        // Init plane
        {
            GLCall(glGenVertexArrays(1, &plane_VAO));

            GLCall(glGenBuffers(1, &plane_VBO));
            GLCall(glGenBuffers(1, &plane_EBO));
            GLCall(glBindVertexArray(plane_VAO));

            GLCall(glBindBuffer(GL_ARRAY_BUFFER, plane_VBO));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW));

            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_EBO));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW));

            GLCall(glEnableVertexAttribArray(0));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
            GLCall(glEnableVertexAttribArray(1));
            GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
            GLCall(glEnableVertexAttribArray(2));
            GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
            GLCall(glBindVertexArray(0));

            tex_plane_diffuse = new Texture2D("res/textures/elecfrog.jpg");
            tex_plane_diffuse->Bind(5);
            plane_Shader = std::make_shared<Shader>("res/Shaders/shadow_mapping/plane.vert", "res/Shaders/shadow_mapping/plane.frag");
        }

        // InitSkybox
        {
            GLCall(glGenVertexArrays(1, &skybox_VAO));
            GLCall(glGenBuffers(1, &skybox_VBO));
            GLCall(glBindVertexArray(skybox_VAO));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr)));
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            GLCall(glBindVertexArray(0));

            skybox_Shader = std::make_shared<Shader>("res/Shaders/skybox/skybox.vert", "res/Shaders/skybox/skybox.frag");
            skybox_textureCube = std::make_shared<TextureCube>(texCube_Cloud);
            skybox_textureCube->Bind(4);
            skybox_Shader->Unbind();
        }


        // light view  shader get the depth buffer in the first render pass 
        lightView_Shader = std::make_shared<Shader>(
                "res/Shaders/point_shadow/light_view.vert",
                "res/Shaders/point_shadow/light_view.frag",
                "res/Shaders/point_shadow/light_view.geom");

        // configure depth map FBO
        InitDpethCubeMap();
    }

    ~Scene08_PointShadow() {}

    void InitOpenGLFunctions() override
    {
        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
    }

    void OnUpdate(float _deltaTime = 0.0f) override
    {
        // glfwGetTime is called only once, the first time this function is called
        static double lastTime = glfwGetTime();

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        float angle = 20.0f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle) * deltaTime, glm::vec3(1.0f, 0.3f, 0.5f));

        for (const auto& subMesh : cp_cube->GetMeshes())
        {
            glm::mat4 tmpModel = glm::mat4(1.0f);
            tmpModel = glm::translate(tmpModel, glm::vec3(1.079 + translation[0], 0.169 + translation[1], 1.045 + translation[2]));
            tmpModel = glm::scale(tmpModel, glm::vec3(0.2f * scaling, 0.2f * scaling, 0.2f * scaling));
            subMesh->UpdateModelMatrix(tmpModel);
        }

        for (const auto& subMesh : sf_cube->GetMeshes())
        {

            glm::mat4 tmpModel = glm::mat4(1.0f);
            tmpModel = glm::translate(tmpModel, glm::vec3(0.275 + translation[0], 0.118 + translation[1], 0.908 + translation[2]));
            tmpModel = glm::scale(tmpModel, glm::vec3(0.015f * scaling, 0.015f * scaling, 0.015f * scaling));

            subMesh->UpdateModelMatrix(tmpModel);
        }

        processInput(m_WindowSystem->GetWindowHandle(), deltaTime);

        mainCamera.UpdateCameraMatrix();

        if (reloadShaders)
        {
            scene_Shader = std::make_shared<Shader>(scene_Shader->vertPath, scene_Shader->fragPath);
            plane_Shader = std::make_shared<Shader>(plane_Shader->vertPath, plane_Shader->fragPath);
            skybox_Shader = std::make_shared<Shader>(skybox_Shader->vertPath, skybox_Shader->fragPath);

            reloadShaders = false;
            std::cout << "[Reload Shader]" << std::endl;
        }

        fn_wireframeMode(is_wireframe);
    }

    void OnRender() override
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT), near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms
                {
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f))),
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f))),
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f))),
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f))),
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f))),
                        (shadowProj * glm::lookAt(ToGLMVec3(lightPos), ToGLMVec3(lightPos) + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)))
                };

        glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(ToGLMVec3(lightPos), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // 1. render depth of scene to depth cube map texture (from light's perspective)
        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO));
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));

        RenderLightView(shadowTransforms);

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // reset viewport
        GLCall(glViewport(0, 0, m_WindowSystem->GetWindowWidth(), m_WindowSystem->GetWindowHeight()))
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

        // RenderDepthMap(visiable_debugDepthMap);

        if (visiable_cpCube)
        {
            for (const auto& submesh : cp_cube->GetMeshes())
            {
                scene_Shader->Bind();

                scene_Shader->SetUniformMat4f("M", submesh->mat_transformLocalToWorld);
                scene_Shader->SetUniformMat4f("V", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
                scene_Shader->SetUniformMat4f("P", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

                glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * submesh->mat_transformLocalToWorld;
                scene_Shader->SetUniformMat4f("MV", ModelViewMatrix); // View & Porj Matrix Come from Current Camera

                scene_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera

                // Lighting Relevant
                scene_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
                scene_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));
                scene_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

                const glm::vec3 lightDirection = ToGLMVec3(lightPos) - cp_cube->GetMeshes()[0]->WorldPosition;
                scene_Shader->SetUniform3f("lit.direction", lightDirection);
                scene_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
                scene_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));


                scene_Shader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
                scene_Shader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
                scene_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

                glActiveTexture(GL_TEXTURE5);
                tex_plane_diffuse->Bind(5);
                // Setting Materials
                scene_Shader->SetUniform1i("material.tex_banana_d", 5);
                scene_Shader->SetUniform3f("material.specular", submesh->m_Material->GetMaterialProperites().specularColor);
                scene_Shader->SetUniform1f("material.shininess", submesh->m_Material->GetMaterialProperites().shininess);
                scene_Shader->SetUniform3f("material.diffuse", submesh->m_Material->GetMaterialProperites().diffuseColor);
                scene_Shader->SetUniform3f("material.ambient", submesh->m_Material->GetMaterialProperites().ambientColor);

                scene_Shader->SetUniform1f("far_plane", far_plane);


                // ShadowMapping
                GLCall(glActiveTexture(GL_TEXTURE8));
                GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap));
                scene_Shader->SetUniform1i("shadowMap", 8);

                submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
            }

        }

        if (visiable_sfCube)
        {
            for (const auto& submesh : sf_cube->GetMeshes())
            {
                scene_Shader->Bind();

                scene_Shader->SetUniformMat4f("M", submesh->mat_transformLocalToWorld);
                scene_Shader->SetUniformMat4f("V", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
                scene_Shader->SetUniformMat4f("P", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

                glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * submesh->mat_transformLocalToWorld;
                scene_Shader->SetUniformMat4f("MV", ModelViewMatrix); // View & Porj Matrix Come from Current Camera
                scene_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera

                // Lighting Relevant
                scene_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
                scene_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));
                scene_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

                glm::vec3 lightDirection = ToGLMVec3(lightPos) - sf_cube->GetMeshes()[0]->WorldPosition;
                scene_Shader->SetUniform3f("lit.direction", lightDirection);
                scene_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
                scene_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

                scene_Shader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
                scene_Shader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
                scene_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

                glActiveTexture(GL_TEXTURE5);
                tex_plane_diffuse->Bind(5);
                // Setting Materials
                scene_Shader->SetUniform1i("material.tex_banana_d", 5);
                scene_Shader->SetUniform3f("material.specular", submesh->m_Material->GetMaterialProperites().specularColor);
                scene_Shader->SetUniform1f("material.shininess", submesh->m_Material->GetMaterialProperites().shininess);
                scene_Shader->SetUniform3f("material.diffuse", submesh->m_Material->GetMaterialProperites().diffuseColor);
                scene_Shader->SetUniform3f("material.ambient", submesh->m_Material->GetMaterialProperites().ambientColor);

                scene_Shader->SetUniform1f("far_plane", far_plane);


                // ShadowMapping
                GLCall(glActiveTexture(GL_TEXTURE8));
                GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap));
                scene_Shader->SetUniform1i("shadowMap", 8);

                submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
            }
        }

        if (visiable_plane)
        {
            plane_Shader->Bind();
            glBindVertexArray(plane_VAO);
            glm::mat4 m = glm::mat4(1.f);
            plane_Shader->SetUniformMat4f("M", m);
            plane_Shader->SetUniformMat4f("V", mainCamera.viewMatrix);
            plane_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

            plane_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera

            // Lighting Relevant
            plane_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
            plane_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));
            plane_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

            // Setting Materials
            glActiveTexture(GL_TEXTURE5);
            tex_plane_diffuse->Bind(5);
            plane_Shader->SetUniform1i("tex_Diffuse", 5);
            // ShadowMapping
            GLCall(glActiveTexture(GL_TEXTURE8));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap));
            plane_Shader->SetUniform1i("shadowMap", 8);

            // DrawCall
            glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, (const void*)nullptr);
            plane_Shader->Unbind();
        }

        if (visiable_skybox)
            RenderSkybox();
    }

    void OnImGuiRender() override
    {
        ImGui::Checkbox("Wireframe Mode", &is_wireframe);
        ImGui::Checkbox("visiable_cpCube", &visiable_cpCube);
        ImGui::Checkbox("visiable_sfCube", &visiable_sfCube);
        ImGui::Checkbox("visiable_plane", &visiable_plane);
        ImGui::Checkbox("visiable_depthMap", &visiable_debugDepthMap);
        ImGui::Checkbox("visiable_skyBox", &visiable_skybox);

        if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F')) reloadShaders = true;

        ImGui::ColorEdit3("Light Color", lightColor);
        ImGui::SliderFloat3("Light Position", lightPos, 0.0f, 3.0f);

        ImGui::SliderFloat("InnerCutOffAngle", &innerCutOff, 0.0f, 90.0f);
        ImGui::SliderFloat("OuterCutOffAngle", &outerCutOff, 0.0f, 90.0f);

        ImGui::SliderFloat("Near", &near_plane, 0.0f, 5.0f);
        ImGui::SliderFloat("Far", &far_plane, 5.0f, 10.0f);

        ImGui::SliderFloat("X", &mainCamera.cameraPos.x, 0.f, 1.0f);
        ImGui::SliderFloat("Y", &mainCamera.cameraPos.y, 0.f, 1.0f);
        ImGui::SliderFloat("Z", &mainCamera.cameraPos.z, 0.f, 1.0f);
        ImGui::SliderFloat("Sensitivity", &mainCamera.keySensitivity, 0.01f, 2.0f);
        ImGui::SliderFloat("Yaw Angle", &mainCamera.yaw, 0.0f, 360.0f);
        ImGui::SliderFloat("Pitch Angle", &mainCamera.pitch, -90.0f, 90.0f);
        ImGui::SliderFloat("FOV", &mainCamera.FOV, 10.0f, 120.0f);

        ImGui::SliderFloat3("Translation", translation, -3.0f, 3.0f);
        ImGui::SliderFloat("Scale", &scaling, 0.0f, 1.0f);

        ImGui::Text("W/A/S/D to control Camera Move");
        ImGui::Text("Q/E to control Camera Yaw");
        ImGui::Text("Ctrl+Q/E to control Camera Pitch");
        ImGui::Text("Alt+Q/E to control Camera FOV");
    }

    void processInput(GLFWwindow* window, float deltaTime)
    {
        float cameraSpeed = 0.05f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            mainCamera.cameraPos += mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            mainCamera.cameraPos -= mainCamera.keySensitivity * cameraSpeed * mainCamera.cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(glm::cross(mainCamera.cameraFront, mainCamera.cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
            mainCamera.cameraPos -= mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            mainCamera.cameraPos += mainCamera.keySensitivity * glm::normalize(mainCamera.cameraUp) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
            mainCamera.yaw -= mainCamera.keySensitivity * 5.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
            mainCamera.yaw += mainCamera.keySensitivity * 5.0f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            mainCamera.pitch -= mainCamera.keySensitivity * 5.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            mainCamera.pitch += mainCamera.keySensitivity * 5.0f;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            mainCamera.FOV -= mainCamera.keySensitivity * 2.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            mainCamera.FOV += mainCamera.keySensitivity * 2.0f;
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
            mainCamera.FOV *= -1.0f;
    }

    void RenderLightView(std::vector<glm::mat4> shadowTransforms)
    {

        for (const auto& submesh : cp_cube->GetMeshes())
        {
            lightView_Shader->Bind();
            lightView_Shader->SetUniformMat4f("M", submesh->mat_transformLocalToWorld);
            lightView_Shader->SetUniform1f("far_plane", far_plane);
            lightView_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));

            for (uint32_t i = 0; i < 6; ++i)
                lightView_Shader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]); // View & Porj Matrix Come from Current Camera
            submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
        }

        for (const auto& submesh : sf_cube->GetMeshes())
        {
            lightView_Shader->Bind();
            lightView_Shader->SetUniform1f("far_plane", far_plane);
            lightView_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));
            for (uint32_t i = 0; i < 6; ++i)
                lightView_Shader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]); // View & Porj Matrix Come from Current Camera

            submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
        }

        {
            lightView_Shader->Bind();
            glBindVertexArray(plane_VAO);

            glm::mat4 M = glm::mat4(1.0f);
            lightView_Shader->SetUniformMat4f("M", M);
            lightView_Shader->SetUniform1f("far_plane", far_plane);
            lightView_Shader->SetUniform3f("lightPos", ToGLMVec3(lightPos));
            for (uint32_t i = 0; i < 6; ++i)
                lightView_Shader->SetUniformMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]); // View & Porj Matrix Come from Current Camera

            glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, (const void*)nullptr);
            lightView_Shader->Unbind();
        }

    }


    void InitDpethCubeMap()
    {

        // unsigned int depthCubemapFBO;

        GLCall(glGenFramebuffers(1, &depthCubemapFBO));

        // unsigned int depthCubemap;
        glGenTextures(1, &depthCubemap);

        // const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // These are very important to prevent seams
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D
                    (
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                            0,
                            GL_DEPTH_COMPONENT,
                            SHADOW_WIDTH,
                            SHADOW_HEIGHT,
                            0,
                            GL_DEPTH_COMPONENT,
                            GL_FLOAT,
                            nullptr
                    );
        }

        // attach depth texture as FBO's depth buffer
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO));
        GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0));
        GLCall(glDrawBuffer(GL_NONE));
        GLCall(glReadBuffer(GL_NONE));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void RenderSkybox()
    {
        // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
        glDepthFunc(GL_LEQUAL);

        skybox_Shader->Bind();

        glm::mat3 v_mat3 = glm::mat3(mainCamera.viewMatrix);
        glm::mat4 v_mat4 = glm::mat4(v_mat3);
        skybox_Shader->SetUniformMat4f("V", v_mat4);
        skybox_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

        glBindVertexArray(skybox_VAO);

        skybox_textureCube->Bind(4);


        glActiveTexture(GL_TEXTURE4);

        skybox_Shader->SetUniform1i("skybox", 4);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // Switch back to the normal depth function
        glDepthFunc(GL_LESS);

        skybox_Shader->Unbind();
    }
};


