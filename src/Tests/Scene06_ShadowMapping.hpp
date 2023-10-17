#pragma once

#include "ScenePreCompiled.h"

class WindowSystem;

class Scene06_ShadowMapping : public Scene
{
    // Model
    std::shared_ptr<Model> cp_cube, sf_cube;

    // Shader
    std::shared_ptr<Shader> scene_Shader;
    std::shared_ptr<Shader> lightView_Shader; // Render in Light View

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

    bool visiable_cpCube{ true };
    bool visiable_sfCube{ true };
    bool visiable_plane{ true };
    bool visiable_skybox{ true };
    bool visiable_debugDepthMap{ false };

    // Skybox
    unsigned int skybox_VAO, skybox_VBO;
    std::shared_ptr<Shader> skybox_Shader;
    std::shared_ptr<TextureCube> skybox_textureCube;

    float scaling{ 1.0f };
    float translation[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };

    // Create plane Objects
    unsigned int plane_VAO, plane_VBO, plane_EBO;
    Texture2D* tex_plane_diffuse;
    std::shared_ptr<Shader> plane_Shader;

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO, depthMap;

    unsigned int quad_VAO, quad_VBO;
    std::shared_ptr<Shader> debugDepthQuad_Shader;

    ResourceManager resourceManager;

public:
    Scene06_ShadowMapping(WindowSystem* windowSystem)
            : Scene(windowSystem)
    {
        m_GameObjects.emplace_back(std::make_shared<elf::GameObject>("Renderer", std::vector<elf::GameOjbectComponent> {
                {
                        elf::GameOjbectComponentType::Transform,
                        elf::ComponentData<elf::GameOjbectComponentType::Transform>(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
                        [] {}
                }
        }));
        m_GameObjects.emplace_back(std::make_shared<elf::GameObject>("Light", std::vector<elf::GameOjbectComponent> {
                {
                        elf::GameOjbectComponentType::Transform, elf::ComponentData<elf::GameOjbectComponentType::Transform>(), [] {}
                },
                {
                        elf::GameOjbectComponentType::Light, elf::ComponentData<elf::GameOjbectComponentType::Light>(glm::vec3(0.0f, 0.836f, 2.889f)), [] {}
                }
        }));



        InitOpenGLFunctions();

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


        {// screen quad VAO
            glGenVertexArrays(1, &quad_VAO);
            glGenBuffers(1, &quad_VBO);
            glBindVertexArray(quad_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        }

        // cp_cube = std::static_pointer_cast<Model>(resourceManager.LoadResource(k_ModelList.at("cpCube_GLTF")));

        cp_cube = Model::LoadModel(k_ModelList.at("cpCube_GLTF"));
        sf_cube = Model::LoadModel(k_ModelList.at("scifiCube_GLTF"));

        // Dealing with subMeshes
        for (const auto& submesh : cp_cube->GetMeshes())
        {
            // Mesh
            submesh->BuildMeshFilter();
            // Material
            submesh->m_Material->GetMaterialProperites().textures.albedo->Bind(0);
            // submesh->m_Material->GetAlbedoTexture()->Bind(0);
        }

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

        // Load Shader
        scene_Shader = std::make_shared<Shader>("res/Shaders/shadow_mapping/render.vert", "res/Shaders/shadow_mapping/render.frag");
        debugDepthQuad_Shader = std::make_shared<Shader>("res/Shaders/shadow_mapping/debug_depth_quad.vert", "res/Shaders/shadow_mapping/debug_depth_quad.frag");
        lightView_Shader = std::make_shared<Shader>("res/Shaders/shadow_mapping/light_view.vert", "res/Shaders/shadow_mapping/light_view.frag");

        {// Init Skybox
            GLCall(glGenVertexArrays(1, &skybox_VAO));
            GLCall(glGenBuffers(1, &skybox_VBO));
            GLCall(glBindVertexArray(skybox_VAO));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO));
            GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr)));
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            GLCall(glBindVertexArray(0));

            skybox_Shader = std::make_shared<Shader>("res/Shaders/Scene08/Skybox.vert", "res/Shaders/Scene08/Skybox.frag");
            skybox_textureCube = std::make_shared<TextureCube>(texCube_Cloud);
            skybox_textureCube->Bind(4);
        }

        // configure depth map FBO
        {
            GLCall(glGenFramebuffers(1, &depthMapFBO));
            // create depth texture
            GLCall(glGenTextures(1, &depthMap));
            GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));


            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
            const float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

            // attach depth texture as FBO's depth buffer
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
            GLCall(glDrawBuffer(GL_NONE));
            GLCall(glReadBuffer(GL_NONE));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

            debugDepthQuad_Shader->Bind();
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            debugDepthQuad_Shader->SetUniform1i("depthMap", 8);
        }
    }

    ~Scene06_ShadowMapping() {}

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
            debugDepthQuad_Shader = std::make_shared<Shader>(debugDepthQuad_Shader->vertPath, debugDepthQuad_Shader->fragPath);
            plane_Shader = std::make_shared<Shader>(plane_Shader->vertPath, plane_Shader->fragPath);
            reloadShaders = false;
            std::cout << "[Reload Shader]" << std::endl;
        }

        fn_wireframeMode(is_wireframe);
    }

    void OnRender() override
    {
        glm::vec3 tmp_LighPosition{};
        for (auto c : m_GameObjects[1]->components)
        {
            if (c.type == elf::GameOjbectComponentType::Light)
            {
                auto* lightData = std::get_if<elf::ComponentData<elf::GameOjbectComponentType::Light>>(&c.data);
                if (lightData != nullptr)
                {
                    tmp_LighPosition = lightData->position;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(tmp_LighPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;


        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));

        RenderLightView(lightSpaceMatrix);

        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        // 2. reset viewport, and then render the scene
        GLCall(glViewport(0, 0, m_WindowSystem->GetWindowWidth(), m_WindowSystem->GetWindowHeight()))

    	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))

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
                scene_Shader->SetUniform3f("lightPos", tmp_LighPosition);
                scene_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

                glm::vec3 lightDirection = tmp_LighPosition - cp_cube->GetMeshes()[0]->WorldPosition;
                scene_Shader->SetUniform3f("lit.direction", lightDirection);
                scene_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
                scene_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

                scene_Shader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
                scene_Shader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
                scene_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

                // Setting Materials
                GLCall(glActiveTexture(GL_TEXTURE5));
                tex_plane_diffuse->Bind(5);
                // GLCall(glBindTexture(GL_TEXTURE_2D, tex_plane_diffuse));

                scene_Shader->SetUniform1i("material.tex_banana_d", 5);
                scene_Shader->SetUniform3f("material.specular", submesh->m_Material->GetMaterialProperites().specularColor);
                scene_Shader->SetUniform1f("material.shininess", submesh->m_Material->GetMaterialProperites().shininess);
                scene_Shader->SetUniform3f("material.diffuse", submesh->m_Material->GetMaterialProperites().diffuseColor);
                scene_Shader->SetUniform3f("material.ambient", submesh->m_Material->GetMaterialProperites().ambientColor);

                // ShadowMapping
                scene_Shader->SetUniform1i("shadowMap", 8);
                GLCall(glActiveTexture(GL_TEXTURE8));
                GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));

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
                scene_Shader->SetUniform3f("lightPos", tmp_LighPosition);
                scene_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

                glm::vec3 lightDirection = tmp_LighPosition - sf_cube->GetMeshes()[0]->WorldPosition;
                scene_Shader->SetUniform3f("lit.direction", lightDirection);
                scene_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
                scene_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

                scene_Shader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
                scene_Shader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
                scene_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

                // Setting Materials
                scene_Shader->SetUniform1i("material.tex_banana_d", 5);
                scene_Shader->SetUniform3f("material.specular", submesh->m_Material->GetMaterialProperites().specularColor);
                scene_Shader->SetUniform1f("material.shininess", submesh->m_Material->GetMaterialProperites().shininess);
                scene_Shader->SetUniform3f("material.diffuse", submesh->m_Material->GetMaterialProperites().diffuseColor);
                scene_Shader->SetUniform3f("material.ambient", submesh->m_Material->GetMaterialProperites().ambientColor);

                // ShadowMapping
                scene_Shader->SetUniform1i("shadowMap", 8);
                GLCall(glActiveTexture(GL_TEXTURE8));
                GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));


                submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
            }

        }

        if (visiable_plane)
        {
            /* Draw */
            {
                plane_Shader->Bind();
                glBindVertexArray(plane_VAO);
                glm::mat4 I = glm::mat4(1.0f);
                plane_Shader->SetUniformMat4f("M", I);
                plane_Shader->SetUniformMat4f("V", mainCamera.viewMatrix);
                plane_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

                plane_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera

                // Lighting Relevant
                plane_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
                plane_Shader->SetUniform3f("lightPos", tmp_LighPosition);
                plane_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

                // Setting Materials
                glActiveTexture(GL_TEXTURE5);
                tex_plane_diffuse->Bind(5);
                plane_Shader->SetUniform1i("tex_Diffuse", 5);
                // ShadowMapping
                plane_Shader->SetUniform1i("shadowMap", 8);
                GLCall(glActiveTexture(GL_TEXTURE8));
                GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));

                // DrawCall
                glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, (const void*)nullptr);
                plane_Shader->Unbind();
            }

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

    void RenderLightView(glm::mat4 lightSpaceMatrix)
    {

        for (const auto& submesh : cp_cube->GetMeshes())
        {
            lightView_Shader->Bind();
            lightView_Shader->SetUniformMat4f("M", submesh->mat_transformLocalToWorld);
            lightView_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera
            submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
        }

        for (const auto& submesh : sf_cube->GetMeshes())
        {
            lightView_Shader->Bind();
            lightView_Shader->SetUniformMat4f("M", submesh->mat_transformLocalToWorld);
            lightView_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera
            submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
        }

        {
            lightView_Shader->Bind();
            glBindVertexArray(plane_VAO);
            glm::mat4 M = glm::mat4(1.0f);
            lightView_Shader->SetUniformMat4f("M", M);
            lightView_Shader->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix); // View & Porj Matrix Come from Current Camera

            glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, (const void*)nullptr);
            lightView_Shader->Unbind();
        }

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
        glActiveTexture(GL_TEXTURE4);
        skybox_Shader->SetUniform1i("skybox", 4);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // Switch back to the normal depth function
        glDepthFunc(GL_LESS);
    }
};


