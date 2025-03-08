#pragma once

#include "ScenePreCompiled.h"

class Scene04_TwoSpheres : public Scene
{
    // Model
    std::shared_ptr<Model> obj_Model;

    // Camera Relevant
    Camera mainCamera;

	// GUI Variables
	bool is_wireframe = false;
	bool reloadShaders = false;

	float lightColor[3] = { 1.0f, 1.0f, 1.0f };
	float lightPos[3] = { 0.0f, 0.836f, 2.889f };

	float innerCutOff = 7.5f;
	float outerCutOff = 12.5f;

    // Create VAO, VBO, and EBO for the skybox
    unsigned int skyboxVAO, skyboxVBO;
    std::shared_ptr<Shader> skybox_Shader;
    TextureCube* tex_Skybox;

    // Shader
    std::shared_ptr<Shader> geometry_Shader;
    unsigned int geometry_VBO, geometry_VAO;

	// ResourceManager resourceManager;

public:
	Scene04_TwoSpheres(WindowSystem* windowSystem)
		: Scene(windowSystem)
	{
	    // Init OpenGL Feature Functions

		// Enables the Depth Buffer
		glEnable(GL_DEPTH_TEST);
		// Enables Cull Facing
		glEnable(GL_CULL_FACE);
		// Keeps front faces
		glCullFace(GL_BACK);
		// Uses counter clock-wise standard
		glFrontFace(GL_CCW);

	    // Load Model
		obj_Model = std::make_shared<Model>();
		obj_Model->LoadModel("res/Models/X Bot.fbx");
		// obj_Model = std::static_pointer_cast<Model>(resourceManager.LoadResource("res/Models/X Bot.fbx"));
	    // Dealing with subMeshes

	    // Left Sphere
	    const auto& left_sphere = obj_Model->GetMeshes()[0];
	    left_sphere->BuildMeshFilter();
	    left_sphere->m_Material->SetAlbedoTexture(banana_texture_diffuse_0);
	    left_sphere->m_Material->GetAlbedoTexture()->Bind(0);
	    left_sphere->m_Material->SetShader(std::make_shared<Shader>("res/Shaders/Sphere/Sphere.vert", "res/Shaders/Sphere/Sphere.frag"));

	    // Right Sphere
	    const auto& right_sphere = obj_Model->GetMeshes()[1];
	    right_sphere->BuildMeshFilter();
	    right_sphere->m_Material->SetAlbedoTexture(banana_texture_diffuse_0);
	    right_sphere->m_Material->GetAlbedoTexture()->Bind(0);
	    right_sphere->m_Material->SetShader(std::make_shared<Shader>("res/Shaders/Sphere/Sphere.vert", "res/Shaders/Sphere/Sphere.frag"));



	    {// Init Skybox
	        GLCall(glGenVertexArrays(1, &skyboxVAO));
	        GLCall(glGenBuffers(1, &skyboxVBO));
	        GLCall(glBindVertexArray(skyboxVAO));
	        GLCall(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
	        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
	        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	        GLCall(glEnableVertexAttribArray(0));
	        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	        GLCall(glBindVertexArray(0));
	        skybox_Shader = std::make_shared<Shader>("res/Shaders/Scene08/Skybox.vert", "res/Shaders/Scene08/Skybox.frag");
	        tex_Skybox = new TextureCube(texCube_CloudSky);
	        tex_Skybox->Bind(4);
	    }

		{// Init Geometry Shader
	        glGenBuffers(1, &geometry_VBO);
	        glGenVertexArrays(1, &geometry_VAO);
	        glBindVertexArray(geometry_VAO);
	        glBindBuffer(GL_ARRAY_BUFFER, geometry_VBO);
	        glBufferData(GL_ARRAY_BUFFER, sizeof(geometry_points), &geometry_points, GL_STATIC_DRAW);
	        glEnableVertexAttribArray(0);
	        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	        glEnableVertexAttribArray(1);
	        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	        glBindVertexArray(0);

	        geometry_Shader = std::make_shared<Shader>("res/Shaders/Geometry/geomTest.vert", 
														"res/Shaders/Geometry/geomTest.frag",
														"res/Shaders/Geometry/geomTest.geom"
														);
		}

	}

	~Scene04_TwoSpheres() {}

	void OnUpdate(float _deltaTime = 0.0f) override
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		float angle = 20.0f;
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle) * deltaTime, glm::vec3(1.0f, 0.3f, 0.5f));

		for(const auto& subMesh: obj_Model->GetMeshes())
		{
			subMesh->UpdateModelMatrix(glm::mat4(1.0f));
		}

		processInput(m_WindowSystem->GetWindowHandle(), deltaTime);

		mainCamera.UpdateCameraMatrix();
	}

	void OnRender() override
	{
		fn_wireframeMode(is_wireframe);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (reloadShaders)
		{
		    obj_Model->GetMeshes()[0]->m_Material->SetShader(std::make_shared<Shader>("res/Shaders/Sphere/Sphere.vert", "res/Shaders/Sphere/Sphere.frag"));
		    obj_Model->GetMeshes()[1]->m_Material->SetShader(std::make_shared<Shader>("res/Shaders/Sphere/Sphere.vert", "res/Shaders/Sphere/Sphere.frag"));

		    skybox_Shader = std::make_shared<Shader>(skybox_Shader->vertPath, skybox_Shader->fragPath);
			reloadShaders = false; std::cout<< "[Reload Shader]" << std::endl;
		}

		// Render Left Sphere
		{
		    const auto& obj_Shader = obj_Model->GetMeshes()[0]->m_Material->GetShader();
			obj_Shader->Bind();

			obj_Shader->SetUniformMat4f("M", obj_Model->GetMeshes()[0]->mat_transformLocalToWorld);
			obj_Shader->SetUniformMat4f("V", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
			obj_Shader->SetUniformMat4f("P", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

			glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * obj_Model->GetMeshes()[0]->mat_transformLocalToWorld;
			obj_Shader->SetUniformMat4f("MV", ModelViewMatrix); // View & Porj Matrix Come from Current Camera

			// Lighting Relevant
			obj_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			obj_Shader->SetUniform3f("lightPos", lightPos[0],lightPos[1],lightPos[2]);
			obj_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

			glm::vec3 lightDirection = glm::vec3(lightPos[0],lightPos[1],lightPos[2]) - obj_Model->GetMeshes()[0]->WorldPosition;
			obj_Shader->SetUniform3f("lit.direction", lightDirection);
			obj_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
			obj_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

			obj_Shader->SetUniform3f("lit.ambient",  0.2f, 0.2f, 0.2f);
			obj_Shader->SetUniform3f("lit.diffuse",  0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
			obj_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

			// Setting Materials
			obj_Shader->SetUniform1i("material.tex_banana_d", 0);
			obj_Shader->SetUniform3f("material.specular", obj_Model->GetMeshes()[0]->m_Material->GetMaterialProperites().specular);
			obj_Shader->SetUniform1f("material.shininess", obj_Model->GetMeshes()[0]->m_Material->GetMaterialProperites().shininess);
			obj_Shader->SetUniform3f("material.diffuse", obj_Model->GetMeshes()[0]->m_Material->GetMaterialProperites().diffuse);
			obj_Shader->SetUniform3f("material.ambient", obj_Model->GetMeshes()[0]->m_Material->GetMaterialProperites().ambient);

			// Setting Materials
			tex_Skybox->Bind(4);
			glActiveTexture(GL_TEXTURE4);
			obj_Shader->SetUniform1i("skybox", 4);

			obj_Model->GetMeshes()[0]->DrawCall(DrawCallType::ELE_TRIANGLE);
		}

		// Render Right Sphere
		{
		    const auto& obj_Shader = obj_Model->GetMeshes()[1]->m_Material->GetShader();

			obj_Shader->Bind();

			obj_Shader->SetUniformMat4f("M", obj_Model->GetMeshes()[1]->mat_transformLocalToWorld);
			obj_Shader->SetUniformMat4f("V", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
			obj_Shader->SetUniformMat4f("P", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

			glm::mat4 ModelViewMatrix = mainCamera.viewMatrix * obj_Model->GetMeshes()[1]->mat_transformLocalToWorld;
			obj_Shader->SetUniformMat4f("MV", ModelViewMatrix); // View & Porj Matrix Come from Current Camera

			// Lighting Relevant
			obj_Shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			obj_Shader->SetUniform3f("lightPos", lightPos[0],lightPos[1],lightPos[2]);
			obj_Shader->SetUniform3f("viewPos", mainCamera.cameraPos);

			glm::vec3 lightDirection = glm::vec3(lightPos[0],lightPos[1],lightPos[2]) - obj_Model->GetMeshes()[1]->WorldPosition;
			obj_Shader->SetUniform3f("lit.direction", lightDirection);
			obj_Shader->SetUniform1f("lit.innerCutOff", glm::cos(glm::radians(innerCutOff)));
			obj_Shader->SetUniform1f("lit.outerCutOff", glm::cos(glm::radians(outerCutOff)));

			obj_Shader->SetUniform3f("lit.ambient",  0.2f, 0.2f, 0.2f);
			obj_Shader->SetUniform3f("lit.diffuse",  0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
			obj_Shader->SetUniform3f("lit.specular", 1.0f, 1.0f, 1.0f);

			// Setting Materials
			obj_Shader->SetUniform1i("material.tex_banana_d", 0);
			obj_Shader->SetUniform3f("material.specular", obj_Model->GetMeshes()[1]->m_Material->GetMaterialProperites().specular);
			obj_Shader->SetUniform1f("material.shininess", obj_Model->GetMeshes()[1]->m_Material->GetMaterialProperites().shininess);
			obj_Shader->SetUniform3f("material.diffuse", obj_Model->GetMeshes()[1]->m_Material->GetMaterialProperites().diffuse);
			obj_Shader->SetUniform3f("material.ambient", obj_Model->GetMeshes()[1]->m_Material->GetMaterialProperites().ambient);

			// Setting Materials
			tex_Skybox->Bind(4);
			glActiveTexture(GL_TEXTURE4);
			obj_Shader->SetUniform1i("skybox", 4);

			obj_Model->GetMeshes()[1]->DrawCall(DrawCallType::ELE_TRIANGLE);
		}

		{
		    // draw points
		    geometry_Shader->Bind();

		    geometry_Shader->SetUniformMat4f("V", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
		    geometry_Shader->SetUniformMat4f("P", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

		    glBindVertexArray(geometry_VAO);
		    // glDrawArrays(GL_POINTS, 0, 4);
		}

		{// Draw Skybox Here
		    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
		    glDepthFunc(GL_LEQUAL);

		    skybox_Shader->Bind();

		    skybox_Shader->SetUniformMat4f("V", glm::mat4(glm::mat3(mainCamera.viewMatrix)));
		    skybox_Shader->SetUniformMat4f("P", mainCamera.projMatrix);

		    glBindVertexArray(skyboxVAO);
		    glActiveTexture(GL_TEXTURE4);
		    skybox_Shader->SetUniform1i("skybox", 4);

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

		ImGui::ColorEdit3("Light Color", lightColor);
		ImGui::SliderFloat3("Light Position", lightPos, 0.0f, 3.0f);
		ImGui::ColorEdit3("Light Color", lightColor);

		ImGui::SliderFloat("InnerCutOffAngle", &innerCutOff, 0.0f, 90.0f); ImGui::SameLine();
		ImGui::SliderFloat("OuterCutOffAngle", &outerCutOff, 0.0f, 90.0f);

		ImGui::SliderFloat("Sensitivity", &mainCamera.keySensitivity, 0.01f, 2.0f);
		ImGui::SliderFloat("Yaw Angle", &mainCamera.yaw, 0.0f, 360.0f);
		ImGui::SliderFloat("Pitch Angle", &mainCamera.pitch, -90.0f, 90.0f);
		ImGui::SliderFloat("FOV", &mainCamera.FOV, 10.0f, 120.0f);

		ImGui::Text("W/A/S/D to control Camera Move");
		ImGui::Text("Q/E to control Camera Yaw");
		ImGui::Text("Ctrl+Q/E to control Camera Pitch");
		ImGui::Text("Alt+Q/E to control Camera FOV");
	}

	void processInput(GLFWwindow *window, float deltaTime)
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
		  mainCamera.FOV *=  -1.0f;
		}

	};
