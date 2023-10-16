#include "Function/Render/Mesh.h"
#include "Function/Render/Model.h"
#include "Platform/FileSystem/FileSystem.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "tiny_obj_loader.h"

void Model::LoadOBJ(const std::string& filename)
{
	tinyobj::attrib_t attrib;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	FilePath path = filename.c_str();
	bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), path.parent_path().string().c_str(), true);
	if(!success)
	{
		//TODO: SPW_LOG_CRITICAL(ERROR);
		throw std::logic_error(err.c_str());
	}

	bool singleMesh = shapes.size() == 1;


	if (attrib.texcoord_ws.empty())
	{
		// BP;
	}

	for (unsigned int shapeIndex = 0; shapeIndex < shapes.size(); ++shapeIndex)
	{
		auto& shape = shapes[shapeIndex];
		std::shared_ptr<Mesh> tmp = std::make_shared<Mesh>();

		//TODO: Using uniqueVertices;
		// std::unordered_map<AttribVertex, Count> uniqueVertices;

		// define the index offset
		Count index_offset = 0;
		
		// load attribute vertices
		for (Count f = 0; f < shape.mesh.num_face_vertices.size(); f++) 
		{
			Count fnum = shape.mesh.num_face_vertices[f];
		
			// For each vertex in the face
			for (Count v = 0; v < fnum; v++)
			{
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				// check the validation of Normals
				glm::vec3 tmp_normal = (attrib.normals.empty()) ?
					glm::vec3(0.0f, 0.0f, 0.0f) :
				// TODO: GenNormals Method
					glm::vec3{ attrib.normals[3 * idx.normal_index], attrib.normals[3 * idx.normal_index + 1],attrib.normals[3 * idx.normal_index + 2] };

				// TODO: GenTangents Method
				glm::vec3 tmp_tangent = glm::vec3(0.0f, 0.0f, 0.0f);

				// check the validation of UVs
				glm::vec2 currentUV = (int(idx.texcoord_index) == -1 || attrib.texcoords.empty()) ?
					glm::vec2(0.0f, 0.0f) :
					glm::vec2{ attrib.texcoords[2 * idx.texcoord_index], 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1] };

				tmp->m_Vertices.emplace_back(
					AttribVertex{
						glm::vec3{attrib.vertices[3 * idx.vertex_index], attrib.vertices[3 * idx.vertex_index + 1],attrib.vertices[3 * idx.vertex_index + 2]},
						tmp_normal,
						tmp_tangent,
						currentUV
					});

				tmp->m_Indices.emplace_back(index_offset + v);
			}
			index_offset += fnum;
		}

		// load materials
		MaterialProperties defaultMaterial;

		std::shared_ptr<Material> tmpMaterial = std::make_shared<Material>();
		tmp->m_Material = std::move(tmpMaterial);

		// TODO VAO, VBO, EBO bindings
		m_Meshes.emplace_back(tmp);
	}
}
