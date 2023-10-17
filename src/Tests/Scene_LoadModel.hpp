#pragma once

#include "ScenePreCompiled.h"
#include "src/Function/Render/Prototype/Cylinder.hpp"
#include "Function/Base/Transform.hpp"
#include "Core/Maths/GaussianKernel.hpp"

unsigned int quadVAO = 0;
unsigned int quadVBO;


glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] =
{
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
};

inline void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

class TextureManager
{
public:
	// Texture | Slot
	std::unordered_map<std::uint32_t, std::uint32_t> textureIdSlotMap;

	uint32_t counter = 0;

	void Include(std::uint32_t id, std::uint32_t slot)
	{
		textureIdSlotMap.emplace(id, slot);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Include(std::uint32_t id/*, std::uint32_t slot*/)
	{
		textureIdSlotMap.emplace(id, counter);

		glActiveTexture(GL_TEXTURE0 + counter);
		glBindTexture(GL_TEXTURE_2D, id);
		counter++;
	}

	void Update(std::uint32_t id)
	{
		glActiveTexture(GL_TEXTURE0 + textureIdSlotMap[id]);
		glBindTexture(GL_TEXTURE_2D, id);
	}


	uint32_t GetSlot(std::uint32_t id)
	{
		return textureIdSlotMap.at(id);
	}
};

class Scene_LoadModel : public Scene
{
	// Model
	std::shared_ptr<Model> scene_Model;
	Transform scene_Transform;

	// Shader
	std::shared_ptr<Shader> scene_Shader;

	std::shared_ptr<Shader> shaderLightingPass;
	std::shared_ptr<Shader> shaderSSAO;
	std::shared_ptr<Shader> shaderSSAOBlur;

	std::shared_ptr<Shader> lightShader;

	std::shared_ptr<Shader> equirectangularToCubemapShader;

	// // Camera
	// Camera mainCamera{glm::vec3(1.45, 0.348, 2.021), 240.f, -11.45f, 45.0f, 0.01f};

	// GUI Variables
	bool is_wireframe = false;
	bool reloadShaders = false;

	float innerCutOff = 7.5f;
	float outerCutOff = 12.5f;

	// skybox Members
	unsigned int skybox_VAO, skybox_VBO;
	std::shared_ptr<Shader> skybox_Shader;
	std::shared_ptr<TextureCube> skybox_textureCube;

	// blur shader
	std::shared_ptr<Shader> bloom_Shader;


	// plane
	Plane planeObject;
	// std::shared_ptr<Mesh> planeMesh;

	// sphere
	UVSphere sphereObject;

	// sphere
	std::shared_ptr<Cylinder> cylinderObject;

	// MRT
	std::shared_ptr<FrameBuffer> mrtFBO;


	TextureManager textureManager;


	// Bloom Effects
	std::shared_ptr<FrameBuffer> BlurHFBO;
	std::shared_ptr<FrameBuffer> BlurVFBO;
	glm::mat3 BlurKernel = Maths::GaussianKernelMat3(1.5);
	float BlurScaleKernel = 1.5f;
	float PreScaleKernel = 1.5f;
	float BlurScaleSampling = 0.5f;
	float BlurStrength = 0.7f;
	bool bEnableBloom = false;


	// IBL Effects
	unsigned int captureFBO;
	unsigned int captureRBO;
	bool bEnableIBL = false;

	/*
	 * Shaders
	 */
	std::shared_ptr<Shader> BlurShader;
	std::shared_ptr<Shader> CylinderShader;
	std::shared_ptr<Shader> SphereShader;
	std::shared_ptr<Shader> PlaneShader;



public:
	Scene_LoadModel(WindowSystem* windowSystem)
		: Scene(windowSystem)
	{
		InitOpenGLFunctions();
		RegisterInputs();

		// Init Plane
		PlaneShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane/plane.frag"));
		SphereShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane/plane.frag"));

		cylinderObject = std::make_shared<Cylinder>(glm::vec3(0, 0.5f, 0.f), glm::vec3(0.f, 0.001f, 0.f));
		CylinderShader = std::make_shared<Shader>(ShaderPath("plane/plane.vert"), ShaderPath("plane/plane.frag"));

		//load model
		scene_Model = Model::LoadModel(AssetPath("DamagedHelmet/DamagedHelmet.gltf"));
		scene_Transform.position = glm::vec3(0.f, 1.1f, 0.f);
		scene_Transform.rotation = Rotation{glm::vec3(89.43f, .0f, 0.f)};

		// Framebuffers
		auto w_width = windowSystem->GetWindowWidth();
		auto w_height = windowSystem->GetWindowHeight();

		mrtFBO = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 5, true);
		BlurHFBO = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 1, false);
		BlurVFBO = std::make_shared<FrameBuffer>(w_width, w_height, AttachmentFormat::RGBA16F, 1, false);

		const auto& textureIds = mrtFBO->GetTextureIds();
		const auto& bloomHtextureId = BlurHFBO->GetTextureIds();
		const auto& bloomVtextureId = BlurVFBO->GetTextureIds();

		/*
		 * Texture Slot Nightmare Solution:
		 * Matching textureIds and slot indices
		 * */
		textureManager.Include(textureIds[0]);
		textureManager.Include(textureIds[1]);
		textureManager.Include(textureIds[2]);
		textureManager.Include(textureIds[3]);
		textureManager.Include(textureIds[4]);
		textureManager.Include(bloomHtextureId[0]);
		textureManager.Include(bloomVtextureId[0]);

		// Dealing with subMeshes
		for (auto&& mesh : scene_Model->GetMeshes())
		{
			// Mesh
			mesh->BuildMeshes();
			// Material
			auto albedo = mesh->m_Material->m_MaterialProperties.textures.albedo;
			textureManager.Include(albedo->GetTextureId());
			auto normal = mesh->m_Material->m_MaterialProperties.textures.normal;
			textureManager.Include(normal->GetTextureId());
			auto metallic = mesh->m_Material->m_MaterialProperties.textures.metallic;
			textureManager.Include(metallic->GetTextureId());
			auto emissive = mesh->m_Material->m_MaterialProperties.textures.emissive;
			textureManager.Include(emissive->GetTextureId());
		}

		// Load Shader
		scene_Shader = std::make_shared<Shader>(ShaderPath("mrt/mrt.vert"), ShaderPath("mrt/mrt.frag"));

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
		skybox_textureCube = std::make_shared<TextureCube>(texCube_Cloud);
		textureManager.Include(skybox_textureCube->GetRenderId());

		BlurShader = std::make_shared<Shader>(ShaderPath("blur/gaussblur.vert"), ShaderPath("blur/gaussblur.frag"));
		bloom_Shader = std::make_shared<Shader>(ShaderPath("final.vert"),ShaderPath("final.frag"));

		bloom_Shader->Bind();
		bloom_Shader->SetUniform1i("gColor", /*const auto& textureIds = */ textureManager.GetSlot(textureIds[0])/*;*/);
		bloom_Shader->SetUniform1i("bloomBlur", textureManager.GetSlot(bloomVtextureId[0]));
		bloom_Shader->SetUniform1i("ssao", 18);
		bloom_Shader->SetUniform1i("gAlbedo", textureManager.GetSlot(textureIds[4]));
		bloom_Shader->SetUniform1i("gNormal", textureManager.GetSlot(textureIds[3]));

		shaderLightingPass = std::make_shared<Shader>(ShaderPath("final.vert"), ShaderPath("ssao_lighting.frag"));
		lightShader = std::make_shared<Shader>(ShaderPath("light/light.vert"), ShaderPath("light/light.frag"));


		// IBL Effects
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
		equirectangularToCubemapShader = std::make_shared<Shader>(ShaderPath("ibl/cubemap.vert"), ShaderPath("ibl/equirectangular_to_cubemap.frag"));
	}

	~Scene_LoadModel() override = default;

	void InitOpenGLFunctions() override
	{
		//glEnable(GL_DEPTH_TEST);
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


		model_matrix
			= glm::translate(glm::mat4(1.0f), scene_Transform.position)
			* glm::mat4_cast(scene_Transform.rotation.quaternion)
			* glm::scale(glm::mat4(1.0f), scene_Transform.scaling);

		for (const auto& subMesh : scene_Model->GetMeshes())
		{
			subMesh->UpdateModelMatrix(model_matrix);
		}

		ProcessInput(deltaTime);

		mainCamera.UpdateCameraMatrix();

		fn_wireframeMode(is_wireframe);
	}

	void OnRender() override
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (reloadShaders)
		{
			ReloadShader(scene_Shader);
			ReloadShader(PlaneShader);
			ReloadShader(SphereShader);
			ReloadShader(BlurShader);
			ReloadShader(bloom_Shader);
			reloadShaders = false;
			SPW_INFO("Reload Shaders");
		}

		/*
		 * MRT Pass
		 * */
		{
			mrtFBO->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			for (const auto& submesh : scene_Model->GetMeshes())
			{
				scene_Shader->Bind();

				scene_Shader->SetUniformMat4f("M", model_matrix);
				// View & Proj Matrix Come from Current Camera
				scene_Shader->SetUniformMat4f("V", mainCamera.viewMatrix);
				scene_Shader->SetUniformMat4f("P", mainCamera.projMatrix);
				// View & Porj Matrix Come from Current Camera

				glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * model_matrix;
				scene_Shader->SetUniformMat4f("MV", ModelViewMatrix); // View & Porj Matrix Come from Current Camera

				// Lighting Relevant
				scene_Shader->SetUniform3f("lightColor", light.color);
				scene_Shader->SetUniform3f("lightPos", light.position);
				scene_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

				glm::vec3 lightDirection = glm::normalize(light.position - scene_Transform.position);

				scene_Shader->SetUniform3f("lit.direction", lightDirection);
				scene_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
				scene_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

				scene_Shader->SetUniform3f("lit.ambient", 0.2f, 0.2f, 0.2f);
				scene_Shader->SetUniform3f("lit.diffuse", 0.5f, 0.5f, 0.5f);
				scene_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

				// Setting Materials

				// Material
				auto albedo = submesh->m_Material->m_MaterialProperties.textures.albedo;
				auto normal = submesh->m_Material->m_MaterialProperties.textures.normal;
				auto metallic = submesh->m_Material->m_MaterialProperties.textures.metallic;
				auto emissive = submesh->m_Material->m_MaterialProperties.textures.emissive;

				scene_Shader->SetUniform1i("material.albedoTexture", textureManager.GetSlot(albedo->GetTextureId()));
				scene_Shader->SetUniform1i("material.normalTexture", textureManager.GetSlot(normal->GetTextureId()));
				scene_Shader->SetUniform1i("material.metallicRoughnessTexture",
				                           textureManager.GetSlot(metallic->GetTextureId()));
				scene_Shader->SetUniform1i("material.emissiveTexture",
				                           textureManager.GetSlot(emissive->GetTextureId()));

				scene_Shader->SetUniform3f("material.specular",
				                           submesh->m_Material->GetMaterialProperites().specularColor);
				scene_Shader->SetUniform1f("material.shininess",
				                           submesh->m_Material->GetMaterialProperites().shininess);
				scene_Shader->SetUniform3f("material.diffuse",
				                           submesh->m_Material->GetMaterialProperites().diffuseColor);
				scene_Shader->SetUniform3f("material.ambient",
				                           submesh->m_Material->GetMaterialProperites().ambientColor);
				submesh->DrawCall(DrawCallType::ELE_TRIANGLE);
			}
			mrtFBO->Unbind();
		}


		/*
		 * Screen Pass
		 * */
		{
			if (std::fabs(BlurScaleKernel - PreScaleKernel) > 0.1)
			{
				BlurKernel = Maths::GaussianKernelMat3(BlurScaleKernel);
				PreScaleKernel = BlurScaleKernel;
			}

			for (int horizontal = 0; horizontal <= 1; ++horizontal)
			{
				if (horizontal)
				{
					BlurShader->Bind();
					{
						BlurHFBO->Bind();
						BlurShader->SetUniform1i("blurdirection", horizontal);
						BlurShader->SetUniform1i("samplerColor", textureManager.GetSlot(mrtFBO->GetTextureIds()[1]));
						BlurShader->SetUniform1f("blur_sample_scale", BlurScaleSampling);
						BlurShader->SetUniform1f("blurStrength", BlurStrength);
						BlurShader->SetUniformMat3f("blur_kernel", BlurKernel);

						renderQuad();
						BlurHFBO->Unbind();
					}
				}
				else
				{
					BlurShader->Bind();

					BlurVFBO->Bind();
					BlurShader->SetUniform1i("blurdirection", horizontal);

					BlurShader->SetUniform1i("samplerColor", textureManager.GetSlot(BlurHFBO->GetTextureIds()[0]));
					BlurShader->SetUniform1f("blur_sample_scale", BlurScaleSampling);
					BlurShader->SetUniform1f("blurStrength", BlurStrength);
					BlurShader->SetUniformMat3f("blur_kernel", BlurKernel);

					renderQuad();
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
			bloom_Shader->Bind();
			bloom_Shader->SetUniform1i("bloom", bEnableBloom);
			bloom_Shader->SetUniform1i("isSsao", 0);
			bloom_Shader->SetUniform1i("irradianceMap", textureManager.GetSlot(skybox_textureCube->GetRenderId()));
			bloom_Shader->SetUniform1i("gNormal", textureManager.GetSlot(mrtFBO->GetTextureIds()[3]));
			bloom_Shader->SetUniform1i("ibl", bEnableIBL);

			renderQuad();
		}


		// Bind your framebuffer (source)
		mrtFBO->BindRead();
		mrtFBO->BindDraw();
		auto&& window_ext = m_WindowSystem->GetWindowSize();
		glBlitFramebuffer(
			0, 0, window_ext.width, window_ext.height, // Source rectangle
			0, 0, window_ext.width, window_ext.height, // Destination rectangle
			GL_DEPTH_BUFFER_BIT, // Mask indicating what to copy
			GL_NEAREST // Interpolation method
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
			M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
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
		ImGui::Checkbox("Wireframe Mode", &is_wireframe);
		if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F')) reloadShaders = true;

		ImGui::BeginChild("Transform", ImVec2(0, 90));
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", glm::value_ptr(scene_Transform.position), 0.01f);
			ImGui::DragFloat3("Scale", glm::value_ptr(scene_Transform.scaling), 0.01f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(scene_Transform.rotation.euler), 0.01f);
			scene_Transform.rotation.quaternion = glm::quat(scene_Transform.rotation.euler);
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Light", ImVec2(0, 90));
		if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.01f);
			ImGui::DragFloat3("Color", glm::value_ptr(light.color), 0.01f);
		}
		ImGui::EndChild();

		ImGui::Separator();

		ImGui::BeginChild("Camera", ImVec2(0, 90));
		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", glm::value_ptr(mainCamera.cameraPos), 0.01f);
		}
		ImGui::EndChild();


		//--------------------------------------------------------
		//bloom scale,bloom strength,bloom
		ImGui::BeginChild("Bloom Effects", ImVec2(0, 120));
		if (ImGui::CollapsingHeader("Bloom Effects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enable Bloom", &bEnableBloom);
			ImGui::SliderFloat("Blur Scale (kernel, effects strength)", &BlurScaleKernel, 0.001f, 3.0f);
			ImGui::SliderFloat("Blur Scale (sampling, effects area)", &BlurScaleSampling, 0.0f, 2.0f);
			// 滑动条，范围从 0.0 到 2.0
			ImGui::SliderFloat("Blur Strength", &BlurStrength, 0.0f, 2.0f); // 滑动条，范围从 0.0 到 2.0
		}
		ImGui::EndChild();

		//--------------------------------------------------------
		ImGui::BeginChild("IBL Effects");
		if (ImGui::CollapsingHeader("IBL Effects", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Checkbox("Enable IBL", &bEnableIBL);
		}
		ImGui::EndChild();


		ImGui::BeginChild("Camera");


		ImGui::SliderFloat("Sensitivity", &mainCamera.keySensitivity, 0.01f, 2.0f);
		ImGui::SliderFloat("Yaw Angle", &mainCamera.yaw, 0.0f, 360.0f);
		ImGui::SliderFloat("Pitch Angle", &mainCamera.pitch, -90.0f, 90.0f);
		ImGui::SliderFloat("FOV", &mainCamera.FOV, 10.0f, 120.0f);

		ImGui::Text("W/A/S/D to control Camera Move");
		ImGui::Text("Q/E to control Camera Yaw");
		ImGui::Text("Ctrl+Q/E to control Camera Pitch");
		ImGui::Text("Alt+Q/E to control Camera FOV");

		ImGui::EndChild();
	}

	void RegisterInputs() override
	{
		Scene::RegisterInputs();
		m_WindowSystem->RegisterOnFramebufferFunc(std::bind(&Scene_LoadModel::onFramebufferSizeChanged, this, std::placeholders::_1, std::placeholders::_2));
	}

	inline void onFramebufferSizeChanged(int w, int h)
	{
		mrtFBO->Resize(w, h);

		BlurHFBO->Resize(w, h);
		BlurVFBO->Resize(w, h);

		// active one by one 
		for (const auto& textureId : mrtFBO->GetTextureIds())
		{
			textureManager.Update(textureId);
		}
		for (const auto& textureId : BlurHFBO->GetTextureIds())
		{
			textureManager.Update(textureId);
		}
		for (const auto& textureId : BlurVFBO->GetTextureIds())
		{
			textureManager.Update(textureId);
		}
	}
};
