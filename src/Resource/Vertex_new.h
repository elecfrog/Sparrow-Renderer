//
// Created by elecfrog on 2023/7/19.
//

#ifndef SPARROW_RENDERER_VERTEX_NEW_H
#define SPARROW_RENDERER_VERTEX_NEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct TexturedVertex_new
{
    glm::vec3 position; // position
    glm::vec3 normal; // normal
    glm::vec3 tangent; // tangent
    glm::vec2 texCoords; // uv

    template<class Archive>
    void serialize(Archive& ar)
    {
        ar(
                cereal::make_nvp("position", position),
                cereal::make_nvp("normal", normal),
                cereal::make_nvp("tangent", tangent),
                cereal::make_nvp("texCoords", texCoords)
        );
    }
};

struct SkinnedVertex_new
{
    glm::vec3 position; // position
    glm::vec3 normal; // normal
    glm::vec3 tangent; // tangent
    glm::vec2 texCoords; // uv

    glm::vec4 boneIDs; // bone IDs
    glm::vec4 weights; // weights
};

// Structure to standard vertices used in the meshes
struct AttribVertex
{
    glm::vec3 position; // position
    glm::vec3 normal; // normal
    glm::vec3 tangent; // tangent
    glm::vec2 texCoords; // uv
};

struct SkinnedVertex
{
    glm::vec3 position; // position
    glm::vec3 normal; // normal
    glm::vec3 tangent; // tangent
    glm::vec2 texCoords; // uv

    glm::vec4 boneIDs; // bone IDs
    glm::vec4 weights; // weights
};

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoord;

    bool operator==(const Vertex& other) const
    {
        return Position == other.Position && TexCoord == other.TexCoord && Normal == other.Normal ;
    }
};


namespace std
{
    template <>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.Position) ^ (hash<glm::vec2>()(vertex.TexCoord) << 1)  ^ (hash<glm::vec3>()(vertex.Normal) << 1) ));
        }
    };
}

#endif //SPARROW_RENDERER_VERTEX_NEW_H
