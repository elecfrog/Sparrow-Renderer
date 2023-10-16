#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "Core/Utils/Utility.hpp"
#include "Resource/Vertex_new.h"



namespace elf
{
	class VertexBuffer
	{
	public:
		// Default Constructor
		VertexBuffer() = default;

		// Delete Default Copy Constructor
		VertexBuffer(const VertexBuffer& other) = delete;

		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VertexBuffer(const void* data, uint32_t size);

		VertexBuffer(std::vector<float>& _vertices);

		VertexBuffer(const std::vector<float>& _vertices);

		VertexBuffer(std::vector<AttribVertex>& _attribVertices);

		VertexBuffer(std::vector<SkinnedVertex>& _attribVertices);

        VertexBuffer(std::vector<TexturedVertex_new> &vertices);

        ~VertexBuffer();

		void Bind() const;

		void Unbind() const;

	private:
		unsigned int m_RenderID;
	};

	using VBO = VertexBuffer;
} // namespace elf
