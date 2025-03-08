#pragma once

#include "ScenePreCompiled.h"


class Scene07_PBR : public Scene
{
public:

	//shader
	std::shared_ptr<Shader> sphere_shader;

	// Model
	std::shared_ptr<Model> scene_model;

	// Shader
	std::shared_ptr<Shader> obj_Shader;

	std::shared_ptr<Texture2D> albedo;
	std::shared_ptr<Texture2D> normal;
	std::shared_ptr<Texture2D> metallic; 
	std::shared_ptr<Texture2D> roughness;
	std::shared_ptr<Texture2D> ao;


	//camera
	Camera mainCamera;

	//gui parameters
	bool is_wireframe = false;
	bool reloadShaders = false;

	std::array<glm::vec3, 1> lightPositions{
		glm::vec3(0.0f, 0.0f, 10.0f),
	};
	std::array<glm::vec3, 1> lightColors{
		glm::vec3(150.0f, 150.0f, 150.0f),
	};


	//parametres about creating sphere
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;


public:
	Scene07_PBR(WindowSystem* windowSystem)
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

		sphere_shader = std::make_shared<Shader>("res/Shaders/pbr.vert", "res/Shaders/pbr.frag");

		std::filesystem::path albedoPath = "res/textures/pbr/rusted_iron/albedo.png";
		albedo = std::make_shared<Texture2D>(albedoPath);

		std::filesystem::path normalPath = "res/textures/pbr/rusted_iron/normal.png";
		normal = std::make_shared<Texture2D>(normalPath);

		std::filesystem::path metallicPath = "res/textures/pbr/rusted_iron/metallic.png";
		metallic = std::make_shared<Texture2D>(metallicPath);

		std::filesystem::path roughnessPath = "res/textures/pbr/rusted_iron/roughness.png";
		roughness = std::make_shared<Texture2D>(roughnessPath);

		std::filesystem::path aoPath = "res/textures/pbr/rusted_iron/ao.png";
		ao = std::make_shared<Texture2D>(aoPath);
	}

	~Scene07_PBR() {}

	void OnUpdate(float deltaTime = 0.0f) override
	{
		// glfwGetTime is called only once, the first time this function is called
		static double lastTime = glfwGetTime();

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		float angle = 20.0f;
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle) * deltaTime, glm::vec3(1.0f, 0.3f, 0.5f));

		processInput(m_WindowSystem->GetWindowHandle(), deltaTime);
		mainCamera.UpdateCameraMatrix();
	}

	void OnRender() override
	{
		fn_wireframeMode(is_wireframe);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sphere_shader->Bind();

		albedo->Bind(0);
		normal->Bind(1);
		metallic->Bind(2);
		roughness->Bind(3);
		ao->Bind(4);

		sphere_shader->SetUniformMat4f("view", mainCamera.viewMatrix); // View & Porj Matrix Come from Current Camera
		sphere_shader->SetUniformMat4f("projection", mainCamera.projMatrix); // View & Porj Matrix Come from Current Camera

		// Lighting Relevant
		sphere_shader->SetUniform3f("lightColors", lightColors[0][0], lightColors[0][1], lightColors[0][2]);
		sphere_shader->SetUniform3f("lightPositions", lightPositions[0][0], lightPositions[0][1], lightPositions[0][2]);
		sphere_shader->SetUniform3f("camPos", mainCamera.cameraPos);

		sphere_shader->setInt("albedoMap", 0);
		sphere_shader->setInt("normalMap", 1);
		sphere_shader->setInt("metallicMap", 2);
		sphere_shader->setInt("roughnessMap", 3);
		sphere_shader->setInt("aoMap", 4);

		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f
				));
				sphere_shader->SetUniformMat4f("model", model);
				createSphere();
			}
		}

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			sphere_shader->SetUniform3f("lightPositions[" + std::to_string(i) + "]", newPos);
			sphere_shader->SetUniform3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			sphere_shader->SetUniformMat4f("model", model);
			createSphere();
		}
	}

	void OnImGuiRender() override 
	{ 
		ImGui::Checkbox("Wireframe Mode", &is_wireframe);

		//ImGui::ColorEdit3("Light Color", ToFArray3(lightColors[0]));
		//ImGui::SliderFloat3("Light Position", ToFArray3(lightPositions[0], 0.0f, 3.0f);

		ImGui::SliderFloat("Sensitivity", &mainCamera.keySensitivity, 0.01f, 2.0f);
		ImGui::SliderFloat("Yaw Angle", &mainCamera.yaw, 0.0f, 360.0f);
		ImGui::SliderFloat("Pitch Angle", &mainCamera.pitch, -90.0f, 90.0f);
		ImGui::SliderFloat("FOV", &mainCamera.FOV, 10.0f, 120.0f);

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

	// renders (and builds at first invocation) a sphere
	void createSphere()
	{
		if (sphereVAO == 0)
		{
			glGenVertexArrays(1, &sphereVAO);

			unsigned int vbo, ebo;
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> uv;
			std::vector<glm::vec3> normals;
			std::vector<unsigned int> indices;

			const unsigned int X_SEGMENTS = 64;
			const unsigned int Y_SEGMENTS = 64;
			const float PI = 3.14159265359f;
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
				{
					float xSegment = (float)x / (float)X_SEGMENTS;
					float ySegment = (float)y / (float)Y_SEGMENTS;
					float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
					float yPos = std::cos(ySegment * PI);
					float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

					positions.push_back(glm::vec3(xPos, yPos, zPos));
					uv.push_back(glm::vec2(xSegment, ySegment));
					normals.push_back(glm::vec3(xPos, yPos, zPos));
				}
			}

			bool oddRow = false;
			for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
			{
				if (!oddRow) // even rows: y == 0, y == 2; and so on
				{
					for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
					{
						indices.push_back(y * (X_SEGMENTS + 1) + x);
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					}
				}
				else
				{
					for (int x = X_SEGMENTS; x >= 0; --x)
					{
						indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
						indices.push_back(y * (X_SEGMENTS + 1) + x);
					}
				}
				oddRow = !oddRow;
			}
			indexCount = static_cast<unsigned int>(indices.size());

			std::vector<float> data;
			for (unsigned int i = 0; i < positions.size(); ++i)
			{
				data.push_back(positions[i].x);
				data.push_back(positions[i].y);
				data.push_back(positions[i].z);
				if (normals.size() > 0)
				{
					data.push_back(normals[i].x);
					data.push_back(normals[i].y);
					data.push_back(normals[i].z);
				}
				if (uv.size() > 0)
				{
					data.push_back(uv[i].x);
					data.push_back(uv[i].y);
				}
			}
			glBindVertexArray(sphereVAO);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
			unsigned int stride = (3 + 2 + 3) * sizeof(float);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		}

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
		}


};




