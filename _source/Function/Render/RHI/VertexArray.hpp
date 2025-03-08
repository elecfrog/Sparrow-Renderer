#pragma once
#include <glad/glad.h>
#include <vector>

#include "Core/Utils/Utility.hpp"
#include "VertexBuffer.h"
#include "VertexBufferLayout.hpp"

namespace elf
{
	class VertexArray
	{
		unsigned int m_rendererId;

	public:
		VertexArray()
		{
			GLCall(glGenVertexArrays(1, &m_rendererId));
		}

		~VertexArray()
		{
			GLCall(glDeleteVertexArrays(1, &m_rendererId));
		}

		void Bind() const
		{
			GLCall(glBindVertexArray(m_rendererId));
		}

		void Unbind() const
		{
			GLCall(glBindVertexArray(0));
		}

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			Bind();
			vb.Bind();
			const std::vector<VertexBufferElement>& elements = layout.GetElements();

			// suppress warning C4312:
			// 'reinterpret_cast': conversion from 'unsigned int' to 'const void *' of greater size
			size_t offset = 0;
			for (unsigned int i = 0; i < elements.size(); i++)
			{
				const auto& element = elements[i];
				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i,
					element.count,
					element.type,
					element.normalized,
					layout.GetStride(),
					reinterpret_cast<const void *>(offset))); // links `VBO` to `VAO`
				offset += size_t(element.count) * GetSizeOfType(element.type);
			}
		}

		void LinkAttrib(VertexBuffer& VBO, unsigned int layout, unsigned int numComponents, unsigned int type,
		                unsigned int stride, void* offset)
		{
			VBO.Bind();
			GLCall(glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset));
			GLCall(glEnableVertexAttribArray(layout));
			VBO.Unbind();
		}

	protected:
	};

	using VAO = VertexArray;
}
