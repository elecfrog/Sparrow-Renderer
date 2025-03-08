//
// Created by dudu on 2023/4/12.
//
#include "VertexBufferLayout.hpp"

namespace elf
{
	template<typename T>
	void VertexBufferLayout::Push(unsigned int count)
	{
//		static_assert(false);
	} // disable general template

	template<>
	void VertexBufferLayout::Push<float>(unsigned int count) /*  */
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void VertexBufferLayout::Push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void VertexBufferLayout::Push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
}