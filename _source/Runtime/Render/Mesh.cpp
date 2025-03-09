#include "Mesh.h"

namespace Sparrow
{
	Mesh::Mesh(std::vector<AttribVertex>& attribVertices, std::vector<unsigned>& indices)
		: transformLocalToWorld(Matrix4f(1.0f))
		, m_Vertices(attribVertices)
		, m_Indices(indices)
	{
	}

	Mesh::Mesh(std::vector<SkinnedVertex>& attribVertices, std::vector<unsigned>& indices)
		: transformLocalToWorld(Matrix4f(1.0f))
		, m_SkinnedVertices(attribVertices)
		, m_Indices(indices)
	{
	}

	void Mesh::Translate(const Vector3f& translate)
	{
		worldPosition += translate;
		for (auto& v : m_Vertices)
		{
			v.position += translate;
		}
	}

	void Mesh::BuildMeshes()
	{
		// Mesh
		if (m_SkinnedVertices.empty())
			BuildMeshFilter();
		else
			BuildSkinnedMeshFilter();
	}

	// What is the mesh filter? https://docs.unity3d.com/Manual/class-MeshFilter.html
	void Mesh::BuildMeshFilter()
	{
		vao = std::make_shared<VAO>();
		vao->Bind();

		vbo = std::make_shared<VBO>(m_Vertices);
		ibo = std::make_shared<IBO>(m_Indices);
		vao->Bind();

		// Linking Vertex Attributes
		vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(AttribVertex), (void*)0);
		vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(AttribVertex), (void*)(sizeof(Vector3f)));
		vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(AttribVertex), (void*)(2 * sizeof(Vector3f)));
		vao->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(AttribVertex), (void*)(3 * sizeof(Vector3f)));

		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
	}

	// What is the mesh filter? https://docs.unity3d.com/Manual/class-MeshFilter.html
	void Mesh::BuildSkinnedMeshFilter()
	{
		vao = std::make_shared<VAO>();
		vao->Bind();

		vbo = std::make_shared<VBO>(m_SkinnedVertices);
		ibo = std::make_shared<IBO>(m_Indices);
		vao->Bind();

		// Vector3f position; // position
		// Vector3f normal; // normal
		// Vector3f tangent; // tangent
		// Vector2f texCoords; // uv
		//
		// Vector4f boneIDs; // bone IDs
		// Vector4f weights; // weights
		// Linking Vertex Attributes
		vao->LinkAttrib(*vbo, 0, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, position));
		vao->LinkAttrib(*vbo, 1, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, normal));
		vao->LinkAttrib(*vbo, 2, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, tangent));
		vao->LinkAttrib(*vbo, 3, 2, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, texCoords));
		vao->LinkAttrib(*vbo, 4, 4, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIDs));
		vao->LinkAttrib(*vbo, 5, 4, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, weights));

		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
	}

	void Mesh::DrawCall(const DrawCallType& drawType)
	{
		vao->Bind();
		if(drawType == DrawCallType::ELE_TRIANGLE)
			GLCall(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, (const void*)nullptr));
		vao->Unbind();
	}

	void Mesh::SetLocalPosition(const Vector3f& newLocalPosition)
	{
		this->localPosition = newLocalPosition;
	}

	void Mesh::UpdateModelMatrix(const Matrix4f& newModelMatrix)
	{
		this->transformLocalToWorld = newModelMatrix;
		this->worldPosition = Vector3f(Vector4f(this->worldPosition, 1.0f) * newModelMatrix);
	}
}