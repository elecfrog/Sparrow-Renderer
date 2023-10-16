#pragma once

#include "ScenePreCompiled.h"


// scientical testing + framebuffer + skybox
class Scene02_RollingCubes : public Scene
{
	// create a OpenGL Objects: VAO, VBO, ShaderProgram
	unsigned int cube_VAO, cube_VBO;

	unsigned int quad_VAO, quad_VBO;

	unsigned int screen_FBO, screen_RBO, screen_texColorBuffer;

	std::shared_ptr<Shader> cube_Shader, outline_Shader, screen_Shader;

	std::shared_ptr<Texture2D> container_Texture;
	std::shared_ptr<Texture2D> frog_Texture;

	bool is_wireframe = false;
	bool reload_Shaders = false;

	float light_color[3] = { 1.0f, 1.0f, 1.0f };

	// Camera Relevant
	Camera sceneCamera;

	TextureCube* tex_Skybox;
	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO;
	Shader* skybox_Shader;

	// std::shared_ptr<WindowSystem> windowSystem;

public:
	Scene02_RollingCubes(WindowSystem* windowSystem)
		: Scene(windowSystem)
	{
		// Enables the Depth Buffer
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		{// Framebuffer Initization Scope
			// Framebuffer Configuration
			glGenFramebuffers(1, &screen_FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, screen_FBO);
			// create a color attachment texture
			glGenTextures(1, &screen_texColorBuffer);
			glBindTexture(GL_TEXTURE_2D, screen_texColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 540, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texColorBuffer, 0);
			// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
			glGenRenderbuffers(1, &screen_RBO);
			glBindRenderbuffer(GL_RENDERBUFFER, screen_RBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 960, 540); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screen_RBO); // now actually attach it
			// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		{// Cube Here
			GLCall(glGenVertexArrays(1, &cube_VAO));

			GLCall(glGenBuffers(1, &cube_VBO));
			GLCall(glBindVertexArray(cube_VAO));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, cube_VBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(cube_AttribVertices), cube_AttribVertices, GL_STATIC_DRAW));

			// Linking Vertex Attributes
			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0));
			GLCall(glEnableVertexAttribArray(0));
			GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));
			GLCall(glEnableVertexAttribArray(1));
			GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float))));
			GLCall(glEnableVertexAttribArray(2));
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
		}

		cube_Shader = std::make_shared<Shader>("res/Shaders/Scene05/Scene05.vert", "res/Shaders/Scene05/Scene05.frag");
		outline_Shader = std::make_shared<Shader>("res/Shaders/Scene05/Outline.vert", "res/Shaders/Scene05/Outline.frag");
		screen_Shader = std::make_shared<Shader>("res/Shaders/Scene05/Screen.vert", "res/Shaders/Scene05/Screen.frag");
		skybox_Shader = new Shader("res/Shaders/Scene08/Skybox.vert", "res/Shaders/Scene08/Skybox.frag");

		container_Texture = std::make_shared<Texture2D>("res/textures/container.jpg");
		frog_Texture = std::make_shared<Texture2D>("res/textures/elecfrog.jpg");

		// skybox_Shader->Bind();
		// skybox_Shader->SetUniform1i("skybox", 0);

		tex_Skybox = new TextureCube(facesCubemap);
		tex_Skybox->Bind(4);
	}

	~Scene02_RollingCubes() {}

	void OnUpdate(float _deltaTime = 0.0f) override
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);
		processInput(m_WindowSystem->GetWindowHandle(), deltaTime);

		sceneCamera.UpdateCameraMatrix();
	}

	void OnRender() override
	{
		fn_wireframeMode(is_wireframe);

		glm::mat4 view = sceneCamera.GetViewMatrix();
		glm::mat4 proj = sceneCamera.GetProjMatrix();



		// render
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, screen_FBO);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (reload_Shaders)
		{
			cube_Shader = std::make_shared<Shader>("res/Shaders/Scene05/Scene05.vert", "res/Shaders/Scene05/Scene05.frag");
			screen_Shader = std::make_shared<Shader>("res/Shaders/Scene05/Screen.vert", "res/Shaders/Scene05/Screen.frag");
			reload_Shaders = false;
		}

		cube_Shader->Bind();
		cube_Shader->SetUniform3f("editColorRGB", light_color[0], light_color[1], light_color[2]);
		cube_Shader->SetUniform1i("container", 0);
		cube_Shader->SetUniform1i("elecfrog", 1);

		cube_Shader->SetUniformMat4f("V", view);
		cube_Shader->SetUniformMat4f("P", proj);

		glBindVertexArray(cube_VAO);
		container_Texture->Bind(0);
		frog_Texture->Bind(1);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			cube_Shader->SetUniformMat4f("M", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		cube_Shader->Unbind();

		outline_Shader->Bind();
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		for (unsigned int i = 0; i < 10; ++i)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			float scale = 1.01f;
			model = glm::scale(model, glm::vec3(scale, scale, scale));
			outline_Shader->SetUniformMat4f("M", model);
			outline_Shader->SetUniformMat4f("V", view);
			outline_Shader->SetUniformMat4f("P", proj);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);



		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screen_Shader->Bind();
		screen_Shader->SetUniform1i("screenTexture", 1);
		glBindVertexArray(quad_VAO);
		glBindTexture(GL_TEXTURE_2D, screen_texColorBuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void OnImGuiRender() override
	{
		ImGui::Checkbox("Wireframe Mode", &is_wireframe);
		if (ImGui::Button("ReloadShader") || ImGui::IsKeyPressed('F'))
			reload_Shaders = true;
		ImGui::ColorEdit3("Object Color", light_color);
		ImGui::SliderFloat("Sensitivity", &sceneCamera.keySensitivity, 0.01f, 2.0f);
		ImGui::SliderFloat("Yaw Angle", &sceneCamera.yaw, 0.0f, 360.0f);
		ImGui::SliderFloat("Pitch Angle", &sceneCamera.pitch, -90.0f, 90.0f);
		ImGui::SliderFloat("FOV", &sceneCamera.FOV, 10.0f, 120.0f);

		ImGui::Text("W/A/S/D to control Camera Move");
		ImGui::Text("Q/E to control Camera Yaw");
		ImGui::Text("Ctrl+Q/E to control Camera Pitch");
		ImGui::Text("Alt+Q/E to control Camera FOV");
	}

	void processInput(GLFWwindow* windowInstance, float deltaTime)
	{
		float cameraSpeed = 0.05f * deltaTime; // adjust accordingly
		if (glfwGetKey(windowInstance, GLFW_KEY_W) == GLFW_PRESS)
			sceneCamera.cameraPos += sceneCamera.keySensitivity * cameraSpeed * sceneCamera.cameraFront;
		if (glfwGetKey(windowInstance, GLFW_KEY_S) == GLFW_PRESS)
			sceneCamera.cameraPos -= sceneCamera.keySensitivity * cameraSpeed * sceneCamera.cameraFront;
		if (glfwGetKey(windowInstance, GLFW_KEY_A) == GLFW_PRESS)
			sceneCamera.cameraPos -= sceneCamera.keySensitivity * glm::normalize(glm::cross(sceneCamera.cameraFront, sceneCamera.cameraUp)) * cameraSpeed;
		if (glfwGetKey(windowInstance, GLFW_KEY_D) == GLFW_PRESS)
			sceneCamera.cameraPos += sceneCamera.keySensitivity * glm::normalize(glm::cross(sceneCamera.cameraFront, sceneCamera.cameraUp)) * cameraSpeed;
		if (glfwGetKey(windowInstance, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
			sceneCamera.yaw -= sceneCamera.keySensitivity * 5.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_ALT) != GLFW_PRESS)
			sceneCamera.yaw += sceneCamera.keySensitivity * 5.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			sceneCamera.pitch -= sceneCamera.keySensitivity * 5.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			sceneCamera.pitch += sceneCamera.keySensitivity * 5.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			sceneCamera.FOV -= sceneCamera.keySensitivity * 2.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			sceneCamera.FOV += sceneCamera.keySensitivity * 2.0f;
		if (glfwGetKey(windowInstance, GLFW_KEY_TAB) == GLFW_PRESS && glfwGetKey(windowInstance, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			sceneCamera.FOV *= -1.0f;
	}
};