#include "VertexBuffer.h"
#include "Resource/Vertex_new.h"

namespace elf
{
	VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	{
		GLCall(glGenBuffers(1, &m_RenderID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(std::vector<float>& _vertices)
	{
		GLCall(glGenBuffers(1, &m_RenderID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& _vertices)
	{
		GLCall(glGenBuffers(1, &m_RenderID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW));
	}

	VertexBuffer::VertexBuffer(std::vector<AttribVertex>& _attribVertices)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, _attribVertices.size() * sizeof(AttribVertex), _attribVertices.data(),
		             GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(std::vector<TexturedVertex_new>& vertices)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(AttribVertex), vertices.data(), GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(std::vector<SkinnedVertex>& _attribVertices)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, _attribVertices.size() * sizeof(SkinnedVertex), _attribVertices.data(),
		             GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RenderID));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
} // namespace elf
