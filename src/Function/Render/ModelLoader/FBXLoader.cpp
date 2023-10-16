#include <OpenFBX/ofbx.h>

#include "Core/Maths/Maths.h"
#include "Core/Maths/Transform.h"
#include "Function/Render/Mesh.h"
#include "Function/Render/Model.h"

// std::string m_FBXModelDirectory;

enum class Orientation
{
    Y_UP,
    Z_UP,
    Z_MINUS_UP,
    X_MINUS_UP,
    X_UP
};

Orientation orientation = Orientation::Y_UP;
float fbx_scale = 1.f;

static ofbx::Vec3 operator-(const ofbx::Vec3& a, const ofbx::Vec3& b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

static ofbx::Vec2 operator-(const ofbx::Vec2& a, const ofbx::Vec2& b)
{
    return { a.x - b.x, a.y - b.y };
}

glm::vec3 FixOrientation(const glm::vec3& v)
{
    switch (orientation)
    {
    case Orientation::Y_UP:
        return glm::vec3(v.x, v.y, v.z);
    case Orientation::Z_UP:
        return glm::vec3(v.x, v.z, -v.y);
    case Orientation::Z_MINUS_UP:
        return glm::vec3(v.x, -v.z, v.y);
    case Orientation::X_MINUS_UP:
        return glm::vec3(v.y, -v.x, v.z);
    case Orientation::X_UP:
        return glm::vec3(-v.y, v.x, v.z);
    }
    return glm::vec3(v.x, v.y, v.z);
}

glm::quat FixOrientation(const glm::quat& v)
{
    switch (orientation)
    {
    case Orientation::Y_UP:
        return glm::quat(v.x, v.y, v.z, v.w);
    case Orientation::Z_UP:
        return glm::quat(v.x, v.z, -v.y, v.w);
    case Orientation::Z_MINUS_UP:
        return glm::quat(v.x, -v.z, v.y, v.w);
    case Orientation::X_MINUS_UP:
        return glm::quat(v.y, -v.x, v.z, v.w);
    case Orientation::X_UP:
        return glm::quat(-v.y, v.x, v.z, v.w);
    }
    return glm::quat(v.x, v.y, v.z, v.w);
}

static void computeTangents(ofbx::Vec3* out, int vertex_count, const ofbx::Vec3* vertices, const ofbx::Vec3* normals, const ofbx::Vec2* uvs)
{
    for (int i = 0; i < vertex_count; i += 3)
    {
        const ofbx::Vec3 v0 = vertices[i + 0];
        const ofbx::Vec3 v1 = vertices[i + 1];
        const ofbx::Vec3 v2 = vertices[i + 2];
        const ofbx::Vec2 uv0 = uvs[i + 0];
        const ofbx::Vec2 uv1 = uvs[i + 1];
        const ofbx::Vec2 uv2 = uvs[i + 2];

        const ofbx::Vec3 dv10 = v1 - v0;
        const ofbx::Vec3 dv20 = v2 - v0;
        const ofbx::Vec2 duv10 = uv1 - uv0;
        const ofbx::Vec2 duv20 = uv2 - uv0;

        const float dir = duv20.x * duv10.y - duv20.y * duv10.x < 0 ? -1.f : 1.f;
        ofbx::Vec3 tangent;
        tangent.x = (dv20.x * duv10.y - dv10.x * duv20.y) * dir;
        tangent.y = (dv20.y * duv10.y - dv10.y * duv20.y) * dir;
        tangent.z = (dv20.z * duv10.y - dv10.z * duv20.y) * dir;
        const float l = 1 / sqrtf(float(tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z));
        tangent.x *= l;
        tangent.y *= l;
        tangent.z *= l;
        out[i + 0] = tangent;
        out[i + 1] = tangent;
        out[i + 2] = tangent;
    }
}

glm::vec2 ToLumosVector(const ofbx::Vec2& vec)
{
    return glm::vec2(float(vec.x), float(vec.y));
}

glm::vec3 ToLumosVector(const ofbx::Vec3& vec)
{
    return glm::vec3(float(vec.x), float(vec.y), float(vec.z));
}

glm::vec4 ToLumosVector(const ofbx::Vec4& vec)
{
    return glm::vec4(float(vec.x), float(vec.y), float(vec.z), float(vec.w));
}

glm::vec4 ToLumosVector(const ofbx::Color& vec)
{
    return glm::vec4(float(vec.r), float(vec.g), float(vec.b), 1.0f);
}

glm::quat ToLumosQuat(const ofbx::Quat& quat)
{
    return glm::quat(float(quat.x), float(quat.y), float(quat.z), float(quat.w));
}

bool IsMeshInvalid(const ofbx::Mesh* aMesh)
{
    return aMesh->getGeometry()->getVertexCount() == 0;
}

Maths::Transform GetTransform(const ofbx::Object* mesh)
{
    auto transform = Maths::Transform();

    ofbx::Vec3 p = mesh->getLocalTranslation();

    glm::vec3 pos = (glm::vec3(static_cast<float>(p.x), static_cast<float>(p.y), static_cast<float>(p.z)));
    transform.SetLocalPosition(FixOrientation(pos));

    ofbx::Vec3 r = mesh->getLocalRotation();
    glm::vec3 rot = FixOrientation(glm::vec3(static_cast<float>(r.x), static_cast<float>(r.y), static_cast<float>(r.z)));
    transform.SetLocalOrientation(glm::quat(glm::vec3(rot.x, rot.y, rot.z)));

    ofbx::Vec3 s = mesh->getLocalScaling();
    glm::vec3 scl = glm::vec3(static_cast<float>(s.x), static_cast<float>(s.y), static_cast<float>(s.z));
    transform.SetLocalScale(scl);

    if (mesh->getParent())
    {
        transform.SetWorldMatrix(GetTransform(mesh->getParent()).GetWorldMatrix());
    }
    else
        transform.SetWorldMatrix(glm::mat4(1.0f));

    return transform;
}


std::shared_ptr<Mesh> LoadMesh(const ofbx::Mesh* fbxMesh, int32_t triangleStart, int32_t triangleEnd)
{
    const int32_t firstVertexOffset = triangleStart * 3;
    const int32_t lastVertexOffset = triangleEnd * 3;
    const int vertexCount = lastVertexOffset - firstVertexOffset + 3;

    auto geom = fbxMesh->getGeometry();
    auto numIndices = geom->getIndexCount();
    int vertex_count = geom->getVertexCount();
    const ofbx::Vec3* vertices = geom->getVertices();
    const ofbx::Vec3* normals = geom->getNormals();
    const ofbx::Vec3* tangents = geom->getTangents();
    const ofbx::Vec4* colours = geom->getColors();
    const ofbx::Vec2* uvs = geom->getUVs();
    const int* materials = geom->getMaterials();
    AttribVertex* tempvertices = new AttribVertex[vertex_count];
    uint32_t* indicesArray = new uint32_t[numIndices];
    ofbx::Vec3* generatedTangents = nullptr;

	std::shared_ptr<Mesh> tmp = std::make_shared<Mesh>();

	int indexCount = 0;
    // SharedPtr<Maths::BoundingBox> boundingBox = CreateSharedPtr<Maths::BoundingBox>();

    auto indices = geom->getFaceIndices();

    if (!tangents && normals && uvs)
    {
        generatedTangents = new ofbx::Vec3[vertex_count];
        computeTangents(generatedTangents, vertex_count, vertices, normals, uvs);
        tangents = generatedTangents;
    }

    auto transform = GetTransform(fbxMesh);

    for (int i = 0; i < vertexCount; i++)
    {
        ofbx::Vec3 cp = vertices[i + firstVertexOffset];

        auto& vertex = tempvertices[i];
        glm::vec3 vertex_position = transform.GetWorldMatrix() * glm::vec4(float(cp.x), float(cp.y), float(cp.z), 1.0f);
        FixOrientation(vertex.position);
        // boundingBox->Merge(vertex.Position);

        glm::vec3 vertex_normal = glm::vec3();
        glm::vec3 vertex_tangent = glm::vec3();
        glm::vec2 vertex_texCoords = glm::vec2();
        if (normals)
            vertex_normal = transform.GetWorldMatrix() * glm::normalize(glm::vec4(float(normals[i + firstVertexOffset].x), float(normals[i + firstVertexOffset].y), float(normals[i + firstVertexOffset].z), 1.0f));
        if (uvs)
            vertex_texCoords = glm::vec2(float(uvs[i + firstVertexOffset].x), 1.0f - float(uvs[i + firstVertexOffset].y));
        // if (colours)
        //     vertex.colors = glm::vec4(float(colours[i + firstVertexOffset].x), float(colours[i + firstVertexOffset].y), float(colours[i + firstVertexOffset].z), float(colours[i + firstVertexOffset].w));

    	if (tangents)
            vertex_tangent = transform.GetWorldMatrix() * glm::normalize(glm::vec4(
	            static_cast<float>(tangents[i + firstVertexOffset].x), 
                static_cast<float>(tangents[i + firstVertexOffset].y),
	            static_cast<float>(tangents[i + firstVertexOffset].z), 
                1.0f));

        FixOrientation(vertex_normal);
        FixOrientation(vertex_tangent);

        tmp->m_Vertices.emplace_back(
            AttribVertex{
				vertex_position,
                vertex_normal,
                vertex_tangent,
                vertex_texCoords
			});
    }

    for (int i = 0; i < vertexCount; i++)
    {
        indexCount++;

        int index = (i % 3 == 2) ? (-indices[i] - 1) : indices[i];
        indicesArray[i] = i; // index;
        tmp->m_Indices.emplace_back(i);
    }


    if (generatedTangents)
        delete[] generatedTangents;
    delete[] tempvertices;
    delete[] indicesArray;

    MaterialProperties defaultMaterial;

    std::shared_ptr<Material> tmpMaterial = std::make_shared<Material>();
    tmp->m_Material = std::move(tmpMaterial);

    return tmp;
}


void Model::LoadFBX(const std::string& _filePath)
{
    std::ifstream file(_filePath, std::ios::binary);
    if (!file) 
    {
        // TODO lOG
        std::cerr << "Failed to open file\n";
    }

    std::vector<uint8_t> buffer(1024); // Read 1024 bytes at a time
    std::size_t read_size = 0;
    std::vector<uint8_t> file_data;

    while (file.read(reinterpret_cast<char*>(buffer.data()), buffer.size())) {
        read_size = static_cast<std::size_t>(file.gcount());
        file_data.insert(file_data.end(), buffer.begin(), buffer.begin() + read_size);
    }

    // Read the remainder of the file
    read_size = static_cast<std::size_t>(file.gcount());
    file_data.insert(file_data.end(), buffer.begin(), buffer.begin() + read_size);

    // Pass the file data to OpenFBX for parsing, and do triangleuate
    ofbx::IScene* scene = ofbx::load(file_data.data(), (int)file_data.size(), static_cast<ofbx::u64>(ofbx::LoadFlags::TRIANGULATE));
    if (!scene) {
        std::cerr << "Failed to parse FBX file\n";
        // TODO lOG
    }

    const ofbx::GlobalSettings* settings = scene->getGlobalSettings();
    switch (settings->UpAxis)
    {
    case ofbx::UpVector_AxisX: orientation = Orientation::X_UP;
	    break;
    case ofbx::UpVector_AxisY: orientation = Orientation::Y_UP;
	    break;
    case ofbx::UpVector_AxisZ: orientation = Orientation::Z_UP;
	    break;
    }

    // const int c = scene->getGeometryCount();
    // for (int i = 0; i < c; ++i) {
    //     ImportGeometry& geom = m_geometries.emplace(m_allocator);
    //     geom.fbx = scene->getGeometry(i);
    // }


    int meshCount = scene->getMeshCount();

    for (int i = 0; i < meshCount; ++i)
    {
        const ofbx::Mesh* fbxMesh = (const ofbx::Mesh*)scene->getMesh(i);
        const auto geometry = fbxMesh->getGeometry();
        const auto trianglesCount = geometry->getVertexCount() / 3;

        if (IsMeshInvalid(fbxMesh))
            continue;

        if (fbxMesh->getMaterialCount() < 2 || !geometry->getMaterials())
        {
            m_Meshes.push_back(LoadMesh(fbxMesh, 0, trianglesCount - 1));
        }
        else
        {
            // Create mesh for each material

            const auto materials = geometry->getMaterials();
            int32_t rangeStart = 0;
            int32_t rangeStartMaterial = materials[rangeStart];
            for (int32_t triangleIndex = 1; triangleIndex < trianglesCount; triangleIndex++)
            {
                if (rangeStartMaterial != materials[triangleIndex])
                {
                    m_Meshes.push_back(LoadMesh(fbxMesh, rangeStart, triangleIndex - 1));

                    // Start a new range
                    rangeStart = triangleIndex;
                    rangeStartMaterial = materials[triangleIndex];
                }
            }
            m_Meshes.push_back(LoadMesh(fbxMesh, rangeStart, trianglesCount - 1));
        }
    }
}

