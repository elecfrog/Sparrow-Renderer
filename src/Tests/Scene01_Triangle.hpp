#pragma once

#include "ScenePreCompiled.h"
#include "Resource/ResourceManager.h"

class Scene01_Triangle final : public Scene
{
	// create a OpenGL Objects: triangleVAO, triangleVBO, ShaderProgram
	uint32_t triangleVAO, triangleVBO, triangleIBO;

	uint32_t rectangleVAO, rectangleVBO, rectangleIBO;

	uint32_t shaderProgram;

	bool renderTriangle{ true };

	struct DrawShape
	{
		bool Triangle;
		bool Rectangle;
		bool TwoTriangles; // TODO
	};

	DrawShape shapes{ true, false, false };
	
	bool is_wireframe{ true };

	float color[3] {50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f};


public:
	Scene01_Triangle(WindowSystem* windowSystem)
		: Scene(windowSystem)
	{

		{// Triangle
			GLCall(glGenVertexArrays(1, &triangleVAO));

			GLCall(glGenBuffers(1, &triangleVBO));
			GLCall(glGenBuffers(1, &triangleIBO));
			GLCall(glBindVertexArray(triangleVAO));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, triangleVBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(simple_triangleVertices), simple_triangleVertices, GL_STATIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIBO));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simple_triangleIndices), simple_triangleIndices, GL_STATIC_DRAW));

			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<const void*>(0)));
			GLCall(glEnableVertexAttribArray(0));
		}

		{// Rectangle
			GLCall(glGenVertexArrays(1, &rectangleVAO));

			GLCall(glGenBuffers(1, &rectangleVBO));
			GLCall(glGenBuffers(1, &rectangleIBO));
			GLCall(glBindVertexArray(rectangleVAO));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(simple_rectangleVertices), simple_rectangleVertices, GL_STATIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectangleIBO));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simple_rectangleIndices), simple_rectangleIndices, GL_STATIC_DRAW));

			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<const void*>(0)));
			GLCall(glEnableVertexAttribArray(0));
		}



		uint32_t vertex_shader;
		GLCall(vertex_shader = glCreateShader(GL_VERTEX_SHADER));
		GLCall(glShaderSource(vertex_shader, 1, &simple_vertexShaderSource, nullptr));
		GLCall(glCompileShader(vertex_shader));
		int success;
		char infoLog[512];
		GLCall(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog));
			spdlog::critical("SHADER::VERTEX::COMPILATION_FAILED {0}", infoLog);
		}

		uint32_t frag_shader;
		GLCall(frag_shader = glCreateShader(GL_FRAGMENT_SHADER));
		GLCall(glShaderSource(frag_shader, 1, &simple_fragmentShaderSource, nullptr));
		GLCall(glCompileShader(frag_shader));
		GLCall(glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success));
		if (!success)
		{
			GLCall(glGetShaderInfoLog(frag_shader, 512, nullptr, infoLog));
			spdlog::critical("SHADER::FRAGMENT::COMPILATION_FAILED {0}", infoLog);
		}

		GLCall(shaderProgram = glCreateProgram());

		GLCall(glAttachShader(shaderProgram, vertex_shader));
		GLCall(glAttachShader(shaderProgram, frag_shader));
		GLCall(glLinkProgram(shaderProgram));
		GLCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success));
		if (!success) 
		{
			GLCall(glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		GLCall(glDeleteShader(vertex_shader));
		GLCall(glDeleteShader(frag_shader));
	}

	~Scene01_Triangle() {}

	void OnUpdate(float deltaTime = 0.0f) override {}

	void OnRender() override
	{
		fn_wireframeMode(is_wireframe);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);

		int uniformLoaction;
		GLCall(uniformLoaction = glGetUniformLocation(shaderProgram, "editColorRGB"));
		glUniform3f(uniformLoaction, color[0], color[1], color[2]);

		if (shapes.Triangle) {
			glBindVertexArray(triangleVAO);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, static_cast<const void*>(nullptr));
		}
		else if(shapes.Rectangle) {
			glBindVertexArray(rectangleVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<const void*>(nullptr));

		}
	}

	void OnImGuiRender() override 
	{ 
		ImGui::Checkbox("Wireframe Mode", &is_wireframe);
		if (ImGui::RadioButton("Draw A Triangle", shapes.Triangle)) {
			shapes.Triangle = true;
			shapes.Rectangle = false;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Draw A Rectangle", shapes.Rectangle)) {
			shapes.Triangle = false;
			shapes.Rectangle = true;
		}
		ImGui::ColorEdit3("Triangle Color", color);
	}
};
