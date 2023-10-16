#pragma once

#include <glad/glad.h>
#include <vector>
#include "Core/Utils/Utility.hpp"

namespace elf
{
	static unsigned int GetSizeOfType(unsigned int type)
	{
	  switch (type)
	  {
			  case GL_FLOAT:         return 4u; // 4u is unsigned int 4, same as follows
			  case GL_UNSIGNED_INT:  return 4u;
			  case GL_UNSIGNED_BYTE: return 1u;
	  }

	  ASSERT(false);
	  return 0;
	}

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
	};

	class VertexBufferLayout
	{
		unsigned int m_stride = 0;
		std::vector<VertexBufferElement> m_elements;

	public:
		VertexBufferLayout() {}

		template<typename T>
		void Push(unsigned int count);

		const std::vector<VertexBufferElement> &GetElements() const
		{
			return m_elements;
		}

		unsigned int GetStride() const
		{
			return m_stride;
		}

	};



}