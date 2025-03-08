#include "Mesh.h"

Mesh::Mesh(std::vector<AttribVertex>& _attribVertices, std::vector<unsigned>& _indices)
	:mat_transformLocalToWorld(glm::mat4(1.0f))
	,m_Vertices(_attribVertices)
	,m_Indices(_indices)
{

}

Mesh::Mesh(std::vector<SkinnedVertex>& _attribVertices, std::vector<unsigned>& _indices)
	:mat_transformLocalToWorld(glm::mat4(1.0f))
	,m_SkinnedVertices(_attribVertices)
	,m_Indices(_indices)
{

}

void Mesh::Translate(const glm::vec3& _translate)
{
	WorldPosition += _translate;
	for (auto& v : m_Vertices)
	{
		v.position += _translate;
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
	m_VAO = std::make_shared<elf::VAO>();
	m_VAO->Bind();

	m_VBO = std::make_shared<elf::VBO>(m_Vertices);
	m_IBO = std::make_shared<IBO>(m_Indices);
	m_VAO->Bind();

	// Linking Vertex Attributes
	m_VAO->LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, sizeof(AttribVertex), (void*)0);
	m_VAO->LinkAttrib(*m_VBO, 1, 3, GL_FLOAT, sizeof(AttribVertex), (void*)(sizeof(glm::vec3)));
	m_VAO->LinkAttrib(*m_VBO, 2, 3, GL_FLOAT, sizeof(AttribVertex), (void*)(2 * sizeof(glm::vec3)));
	m_VAO->LinkAttrib(*m_VBO, 3, 2, GL_FLOAT, sizeof(AttribVertex), (void*)(3 * sizeof(glm::vec3)));

	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
}

// What is the mesh filter? https://docs.unity3d.com/Manual/class-MeshFilter.html
void Mesh::BuildSkinnedMeshFilter()
{
	m_VAO = std::make_shared<elf::VAO>();
	m_VAO->Bind();

	m_VBO = std::make_shared<elf::VBO>(m_SkinnedVertices);
	m_IBO = std::make_shared<IBO>(m_Indices);
	m_VAO->Bind();

	// glm::vec3 position; // position
	// glm::vec3 normal; // normal
	// glm::vec3 tangent; // tangent
	// glm::vec2 texCoords; // uv
	//
	// glm::vec4 boneIDs; // bone IDs
	// glm::vec4 weights; // weights
	// Linking Vertex Attributes
	m_VAO->LinkAttrib(*m_VBO, 0, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, position));
	m_VAO->LinkAttrib(*m_VBO, 1, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, normal));
	m_VAO->LinkAttrib(*m_VBO, 2, 3, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, tangent));
	m_VAO->LinkAttrib(*m_VBO, 3, 2, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, texCoords));
	m_VAO->LinkAttrib(*m_VBO, 4, 4, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, boneIDs)); // Fix this line
	m_VAO->LinkAttrib(*m_VBO, 5, 4, GL_FLOAT, sizeof(SkinnedVertex), (void*)offsetof(SkinnedVertex, weights)); // Fix this line

	m_VAO->Unbind();
	m_VBO->Unbind();
	m_IBO->Unbind();
}

void Mesh::DrawCall(const DrawCallType& _drawtype)
{
	m_VAO->Bind();
	if(_drawtype == DrawCallType::ELE_TRIANGLE)
		GLCall(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, (const void*)nullptr));
	m_VAO->Unbind();
}

void Mesh::SetLocalPosition(const glm::vec3& _newLocalPosition)
{
	this->LocalPosition = _newLocalPosition;
}

void Mesh::UpdateModelMatrix(const glm::mat4& _newModelMatrix)
{
	this->mat_transformLocalToWorld = _newModelMatrix;
	this->WorldPosition = glm::vec3(glm::vec4(this->WorldPosition,1.0f) * _newModelMatrix) ;
}