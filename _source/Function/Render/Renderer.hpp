#pragma once
#include <glad/glad.h>

#include "Core/Utils/Utility.hpp"
#include "RHI/IndexBuffer.h"
#include "RHI/Shader.h"
#include "RHI/VertexArray.hpp"

class MeshRenderer
{
public:
	void Clear() 
	{ 
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
	{
		va.Bind();
		ib.Bind();
		shader.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
};
