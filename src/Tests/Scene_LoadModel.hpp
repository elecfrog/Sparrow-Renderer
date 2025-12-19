glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[]    = {
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
};

class Scene_LoadModel
{
    // Model
    std::shared_ptr<Model> scene_Model;
    Transform              scene_Transform;

    // Shader
    std::shared_ptr<Shader> MrtShader;

    std::shared_ptr<Shader> shaderLightingPass;
    std::shared_ptr<Shader> shaderSSAO;
    std::shared_ptr<Shader> shaderSSAOBlur;

    std::shared_ptr<Shader> lightShader;

    std::shared_ptr<Shader> equirectangularToCubemapShader;

    // skybox Members
    unsigned int                 skybox_VAO, skybox_VBO;
    std::shared_ptr<Shader>      skybox_Shader;
    std::shared_ptr<TextureCube> skybox_textureCube;

    // blur shader
    std::shared_ptr<Shader> SceneShader;

    // Objects
    Plane                     planeObject;    /* Plane */
    UVSphere                  sphereObject;   /* UVSphere */
    std::shared_ptr<Cylinder> cylinderObject; /* Cylinder */

    // MRT
    std::shared_ptr<FrameBuffer> mrtFBO;

    // Bloom Effects
    std::shared_ptr<FrameBuffer> BlurHFBO;
    std::shared_ptr<FrameBuffer> BlurVFBO;
    glm::mat3                    BlurKernel        = Maths::GaussianKernel1DMat3(1.5);
    std::array<double, 9>        BlurKernel1       = Maths::GaussianKernel1D<9>(1.0);
    float                        BlurScaleKernel   = 1.5f;
    float                        PreScaleKernel    = 1.5f;
    float                        BlurScaleSampling = 0.5f;
    float                        BlurStrength      = 0.7f;
    bool                         bEnableBloom      = false;

    // IBL Effects
    unsigned int captureFBO;
    unsigned int captureRBO;
    bool         bEnableIBL = false;

    /*
     * Shaders
     */
    std::shared_ptr<Shader> BlurShader;

  public:
    Scene_LoadModel()
    {
        // load model
        scene_Model              = Model::LoadModel(ModelPath("DamagedHelmet/DamagedHelmet.gltf"));
        scene_Transform.position = glm::vec3(0.f, 1.1f, 0.f);
        scene_Transform.rotation = Rotation{ glm::vec3(89.43f, .0f, 0.f) };

        // Framebuffers
        auto w_width  = windowSystem->GetWindowWidth();
        auto w_height = windowSystem->GetWindowHeight();

        mrtFBO   = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 5, true);
        BlurHFBO = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 1, false);
        BlurVFBO = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 1, false);

        const auto& bloomHtextureId = BlurHFBO->GetTextureIds();
        const auto& bloomVtextureId = BlurVFBO->GetTextureIds();

        // Dealing with subMeshes
        for (auto&& mesh : scene_Model->GetMeshes())
        {
            // Mesh
            mesh->BuildMeshes();
            // Material
            auto&  material_props  = mesh->m_Material->m_MaterialProperties;
            auto&& mr_textures     = mesh->m_Material->m_MaterialProperties.mrTextures;
            auto&& common_textures = mesh->m_Material->m_MaterialProperties.cmTextures;
        }

        // Load Shader
        MrtShader = std::make_shared<Shader>(ShaderPath("mrt/mrt.vert"), ShaderPath("mrt/mrt.frag"));

        // IBL Bloom
        BlurShader  = std::make_shared<Shader>(ShaderPath("blur/gaussblur.vert"), ShaderPath("blur/gaussblur.frag"));
        SceneShader = std::make_shared<Shader>(ShaderPath("final.vert"), ShaderPath("final.frag"));

        SceneShader->Bind();
        SceneShader->SetUniform1u("gColor", textureManager.GetSlot(mrtFBO->GetTextureId(0)));
        SceneShader->SetUniform1u("bloomBlur", textureManager.GetSlot(bloomVtextureId[0]));
        SceneShader->SetUniform1u("ssao", 18);
        SceneShader->SetUniform1u("gAlbedo", textureManager.GetSlot(mrtFBO->GetTextureId(4)));
        SceneShader->SetUniform1u("gNormal", textureManager.GetSlot(mrtFBO->GetTextureId(3)));

        shaderLightingPass = std::make_shared<Shader>(ShaderPath("final.vert"), ShaderPath("ssao_lighting.frag"));
        lightShader        = std::make_shared<Shader>(ShaderPath("light/light.vert"), ShaderPath("light/light.frag"));

        // IBL Effects
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
        equirectangularToCubemapShader = std::make_shared<Shader>(ShaderPath("ibl/cubemap.vert"), ShaderPath("ibl/equirectangular_to_cubemap.frag"));
    }

    void OnRender()
    {
        /*
         * Color Pass 1 : MRT Pass
         * Attach all the Results to the MRT FBO we created
         * */
        {
            mrtFBO->Bind();

            for (auto&& mesh : scene_Model->GetMeshes())
            {
                MrtShader->Bind();

                MrtShader->SetUniformMat4f("M", model_matrix);
                // View & Proj Matrix Come from Current Camera
                MrtShader->SetUniformMat4f("V", mainCamera.viewMatrix);
                MrtShader->SetUniformMat4f("P", mainCamera.projMatrix);

                glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * model_matrix;
                MrtShader->SetUniformMat4f("MV", ModelViewMatrix);

                // Lighting Relevant
                MrtShader->SetUniform3f("lightColor", light.color);
                MrtShader->SetUniform3f("lightPos", light.position);
                MrtShader->SetUniform3f("viewPos", mainCamera.cameraPos);

                glm::vec3 lightDirection = glm::normalize(light.position - scene_Transform.position);

                MrtShader->SetUniform3f("lit.direction", lightDirection);
                MrtShader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
                MrtShader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

                MrtShader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
                MrtShader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
                MrtShader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

                // Setting Materials

                // Material
                auto& material_props = mesh->m_Material->m_MaterialProperties;

                auto albedo = material_props.cmTextures.albedo;
                auto normal = material_props.cmTextures.normal;
                MrtShader->SetUniform1u("material.albedoTexture", textureManager.GetSlot(albedo->GetTextureId()));
                MrtShader->SetUniform1u("material.normalTexture", textureManager.GetSlot(normal->GetTextureId()));

                if (material_props.workFlow == PBRWorkFlow::PBR_WORKFLOW_MR)
                {
                    auto metallic = material_props.mrTextures.metallic;
                    MrtShader->SetUniform1u("material.metallicRoughnessTexture", textureManager.GetSlot(metallic->GetTextureId()));
                }
                else if (material_props.workFlow == PBRWorkFlow::PBR_WORKFLOW_SG)
                {
                    auto specularGlossiness = material_props.sgTextures.specularGlossiness;
                    MrtShader->SetUniform1u("material.sgTex", textureManager.GetSlot(specularGlossiness->GetTextureId()));
                    auto ao = material_props.sgTextures.ao;
                    MrtShader->SetUniform1u("material.aoTex", textureManager.GetSlot(ao->GetTextureId()));
                }
                MrtShader->SetUniform1ui("material.WORKFLOW", static_cast<uint32_t>(material_props.workFlow));
                //                MrtShader->SetUniform1u("material.WORKFLOW", 0);

                if (material_props.cmTextures.emissive != nullptr)
                {
                    auto emissive = material_props.cmTextures.emissive;
                    MrtShader->SetUniform1u("material.emissiveTexture", textureManager.GetSlot(emissive->GetTextureId()));
                }

                auto mat_props = mesh->m_Material->GetMaterialProperites();
                MrtShader->SetUniform3f("material.specular", mat_props.specularColor);
                MrtShader->SetUniform1f("material.shininess", mat_props.shininess);
                MrtShader->SetUniform3f("material.diffuse", mat_props.diffuseColor);
                MrtShader->SetUniform3f("material.ambient", mat_props.ambientColor);
                mesh->DrawCall(DrawCallType::ELE_TRIANGLE);
            }
            mrtFBO->Unbind();
        }

        /*
         * Screen Pass
         * */
        {
            if (std::fabs(BlurScaleKernel - PreScaleKernel) > 0.1)
            {
                BlurKernel     = Maths::GaussianKernel1DMat3(BlurScaleKernel);
                PreScaleKernel = BlurScaleKernel;
            }

            for (int horizontal = 0; horizontal <= 1; ++horizontal)
            {
                if (horizontal)
                {
                    BlurShader->Bind();
                    {
                        BlurHFBO->Bind();
                        BlurShader->SetUniform1i("uBlurDirection", horizontal);
                        // Here is a HACK, must Fit the Index in the Shader
                        BlurShader->SetUniform1i("uBlurSourceImage", textureManager.GetSlot(mrtFBO->GetTextureId(1)));
                        BlurShader->SetUniform1f("uBlurSamplingScale", BlurScaleSampling);
                        BlurShader->SetUniform1f("uBlurStrength", BlurStrength);
                        BlurShader->SetUniformMat3f("uBlurKernel", BlurKernel);

                        RenderQuad();
                        BlurHFBO->Unbind();
                    }
                }
                else
                {
                    BlurShader->Bind();

                    BlurVFBO->Bind();
                    BlurShader->SetUniform1i("uBlurDirection", horizontal);
                    // Here is a HACK, must Fit the Index in the Shader
                    BlurShader->SetUniform1i("uBlurSourceImage", textureManager.GetSlot(BlurHFBO->GetTextureId(0)));
                    BlurShader->SetUniform1f("uBlurSamplingScale", BlurScaleSampling);
                    BlurShader->SetUniform1f("uBlurStrength", BlurStrength);
                    BlurShader->SetUniformMat3f("uBlurKernel", BlurKernel);

                    RenderQuad();
                    BlurVFBO->Unbind();
                }
            }
            glDisable(GL_DEPTH_TEST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, 512, 512);
            glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
            equirectangularToCubemapShader->Bind();
            equirectangularToCubemapShader->SetUniform1i("equirectangularMap",
                                                         textureManager.GetSlot(skybox_textureCube->GetRenderId()));
            equirectangularToCubemapShader->SetUniformMat4f("projection", captureProjection);

            for (unsigned int i = 0; i < 6; ++i)
            {
                equirectangularToCubemapShader->SetUniformMat4f("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                       skybox_textureCube->m_RendererId, 0);
            }

            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0))
                GLCall(glActiveTexture(GL_TEXTURE0 + textureManager.GetSlot(skybox_textureCube->GetRenderId())))
                //            textureManager.Update(skybox_textureCube->GetRenderId());
                GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_textureCube->GetRenderId()));
            GLCall(glViewport(0, 0, m_WindowSystem->GetWindowWidth(), m_WindowSystem->GetWindowHeight()))

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SceneShader->Bind();
            SceneShader->SetUniform1i("bloom", bEnableBloom);
            SceneShader->SetUniform1i("isSsao", 0);
            SceneShader->SetUniform1i("irradianceMap", textureManager.GetSlot(skybox_textureCube->GetRenderId()));
            SceneShader->SetUniform1i("gNormal", textureManager.GetSlot(mrtFBO->GetTextureIds()[3]));
            SceneShader->SetUniform1i("ibl", bEnableIBL);

            RenderQuad();
        }

        // Bind your framebuffer (source)
        mrtFBO->BindRead();
        mrtFBO->BindDraw();
        int window_width  = m_WindowSystem->GetWindowWidth();
        int window_height = m_WindowSystem->GetWindowHeight();
        glBlitFramebuffer(
            0, 0, window_width, window_height, // Source rectangle
            0, 0, window_width, window_height, // Destination rectangle
            GL_DEPTH_BUFFER_BIT,               // Mask indicating what to copy
            GL_NEAREST                         // Interpolation method
        );
        // Unbind your framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        /*
         * Normal Single Render Pass
         * */

        {
            glm::mat4 M = glm::translate(glm::mat4(1.0f), light.position);
            M           = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
            sphereObject.Render(SphereShader, mainCamera, M, light);
        }

        /* Draw plane*/
        auto I = glm::mat4(1.0f);
        planeObject.Render(PlaneShader, mainCamera, I, light);

        /* Draw cylinder*/
        auto cylinderM = glm::mat4(0.5f);
        cylinderObject->Render(*CylinderShader, mainCamera, cylinderM, light);

        {
            // Skybox
            // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            skybox_Shader->Bind();

            auto v_mat3 = glm::mat3(mainCamera.viewMatrix);
            auto v_mat4 = glm::mat4(v_mat3);
            skybox_Shader->SetUniformMat4f("V", v_mat4);
            skybox_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

            glBindVertexArray(skybox_VAO);
            GLCall(glActiveTexture(GL_TEXTURE0 + textureManager.GetSlot(skybox_textureCube->GetRenderId())));

            //            glActiveTexture(GL_TEXTURE22);
            skybox_Shader->SetUniform1i("skybox", textureManager.GetSlot(skybox_textureCube->GetRenderId()));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            // Switch back to the normal depth function
            glDepthFunc(GL_LESS);
        }
    }

    void OnImGuiRender() override
    {
        //--------------------------------------------------------
        // bloom scale,bloom strength,bloom
        ImGui::BeginChild("Bloom Effects", ImVec2(0, 120));
        if (ImGui::CollapsingHeader("Bloom Effects", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable Bloom", &bEnableBloom);
            ImGui::SliderFloat("Blur Scale (kernel, effects strength)", &BlurScaleKernel, 0.001f, 3.0f);
            ImGui::SliderFloat("Blur Scale (sampling, effects area)", &BlurScaleSampling, 0.0f, 10.0f);
            ImGui::SliderFloat("Blur Strength", &BlurStrength, 0.0f, 2.0f);
        }
        ImGui::EndChild();

        //--------------------------------------------------------
        ImGui::BeginChild("IBL Effects");
        if (ImGui::CollapsingHeader("IBL Effects", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Enable IBL", &bEnableIBL);
        }
        ImGui::EndChild();
    }
};