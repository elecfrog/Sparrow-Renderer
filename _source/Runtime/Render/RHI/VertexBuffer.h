#pragma once

#include "Render/Vertex.h"

namespace Sparrow
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

		VertexBuffer(std::vector<Vector3f>& _attribVertices);
		VertexBuffer(Vector<Vector3>& buffer);

		VertexBuffer(std::vector<Vector2f>& _attribVertices);
		VertexBuffer(Vector<Vector2>& buffer);

		VertexBuffer(std::vector<SkinnedVertex>& _attribVertices);

        ~VertexBuffer();

		void Bind() const;

		void Unbind() const;

	private:
		unsigned int m_RenderID;
	};

	using VBO = VertexBuffer;
} // namespace Sparrow
