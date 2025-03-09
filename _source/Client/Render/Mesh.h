#pragma once

#include <Base/BaseDefinition.h>
#include "Vertex.h"
#include "RHI/IndexBuffer.h"
#include "RHI/VertexBuffer.h"
#include "RHI/VertexArray.hpp"
#include "Material.h"

namespace Sparrow
{
	class Mesh
	{
	public:

		// TODO: Object Positions
		Vector3f worldPosition{};
		Vector3f localPosition{};
		Matrix4f transformLocalToWorld{}; // transform itself in local space -> Model Matrix

		/*  Rendering Structure*/
		std::vector<AttribVertex>  m_Vertices;
		std::vector<SkinnedVertex> m_SkinnedVertices;
		std::vector<unsigned int> m_Indices;

		/*  Basic Rendering Data  */
		std::shared_ptr<VAO> vao;
		std::shared_ptr<VBO> vbo;
		std::shared_ptr<IBO> ibo;

		/*  One Mesh pair to one material */
		std::shared_ptr<Material> m_Material;

		void BuildMeshes();
		void BuildMeshFilter();
		void BuildSkinnedMeshFilter();

		Matrix4f localMatrix{}; // transform itself in local space
		Matrix4f renderMatrix{};

		Mesh() = default;
		Mesh(const Mesh& mesh) = default;

		Mesh(std::vector<AttribVertex>& attribVertices, std::vector<unsigned int>& indices);
		Mesh(std::vector<SkinnedVertex>& attribVertices, std::vector<unsigned int>& indices);

		void Translate(const Vector3f& translate);
		void DrawCall(const DrawCallType& drawType);
		void SetLocalPosition(const Vector3f& newLocalPosition);
		void UpdateModelMatrix(const Matrix4f& newModelMatrix);

		[[nodiscard]] MaterialProperties GetMaterialProperties() const { return m_Material->GetMaterialProperites(); }
		[[nodiscard]] MaterialTextures GetMaterialTextures() const { return m_Material->GetTextures(); }
	};
}