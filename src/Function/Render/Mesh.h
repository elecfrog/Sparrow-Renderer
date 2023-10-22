#pragma once

#include <vector>
#include <filesystem>

#include "RHI/IndexBuffer.h"
#include "RHI/VertexBuffer.h"
#include "RHI/VertexArray.hpp"
#include "Function/Render/RenderDefinitions.h"
#include "Material.h"

class Mesh
{
public:

	// TODO: Object Positions
	glm::vec3 WorldPosition{};
	glm::vec3 LocalPosition{};
	glm::mat4 mat_transformLocalToWorld; // transform itself in local space -> Model Matrix

	/*  Rendering Structure*/
	std::vector<AttribVertex>  m_Vertices;
	std::vector<SkinnedVertex> m_SkinnedVertices;
	std::vector<unsigned int> m_Indices;

	/*  Basic Rendering Data  */
	std::shared_ptr<elf::VAO> m_VAO;
	std::shared_ptr<elf::VBO> m_VBO;
	std::shared_ptr<IBO> m_IBO;

	/*  One Mesh pair to one material */
	std::shared_ptr<Material> m_Material;

	void BuildMeshes();
	void BuildMeshFilter();
	void BuildSkinnedMeshFilter();

	glm::mat4 mat_Local{}; // transform itself in local space
	glm::mat4 mat_render{};

	Mesh() = default;
	Mesh(const Mesh& mesh) = default;

	Mesh(std::vector<AttribVertex>& _attribVertices, std::vector<unsigned int>& _indices);
	Mesh(std::vector<SkinnedVertex>& _attribVertices, std::vector<unsigned int>& _indices);

	void Translate(const glm::vec3& _translate);

	void DrawCall(const DrawCallType& _drawtype);

	void SetLocalPosition(const glm::vec3& _newLocalPosition);

	void UpdateModelMatrix(const glm::mat4& _newModelMatrix);

    [[nodiscard]] MaterialProperties GetMaterialProperties() const {return m_Material->GetMaterialProperites();}
};