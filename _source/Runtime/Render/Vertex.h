#pragma once

#include "Core/Base/BaseDefinition.h"
#include <cereal/cereal.hpp>

namespace Sparrow
{
    struct SkinnedVertex_new
    {
        Vector3f position; // position
        Vector3f normal; // normal
        Vector3f tangent; // tangent
        Vector2f texCoords; // uv

        Vector4f boneIDs; // bone IDs
        Vector4f weights; // weights
    };

    // Structure to standard vertices used in the meshes
    struct AttribVertex
    {
        Vector3f position; // position
        Vector3f normal; // normal
        Vector3f tangent; // tangent
        Vector2f texCoords; // uv

        template <class Archive>
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

    struct SkinnedVertex
    {
        Vector3f position; // position
        Vector3f normal; // normal
        Vector3f tangent; // tangent
        Vector2f texCoords; // uv

        Vector4f boneIDs; // bone IDs
        Vector4f weights; // weights
    };

    struct Vertex
    {
        Vector3f position; // position
        Vector3f normal; // normal
        Vector2f texCoord; // texCoords

        bool operator==(const Vertex& other) const
        {
            return position == other.position && texCoord == other.texCoord && normal == other.normal;
        }
    };
}

namespace std
{
    template <>
    struct hash<Sparrow::Vector3f>
    {
        size_t operator()(const Sparrow::Vector3f& v) const
        {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
        }
    };

    template <>
    struct hash<Sparrow::Vector2f>
    {
        size_t operator()(const Sparrow::Vector2f& v) const
        {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1);
        }
    };

    template <>
    struct hash<Sparrow::Vertex>
    {
        size_t operator()(Sparrow::Vertex const& vertex) const
        {
            return ((hash<Sparrow::Vector3f>()(vertex.position) ^
                (hash<Sparrow::Vector2f>()(vertex.texCoord) << 1) ^
                (hash<Sparrow::Vector3f>()(vertex.normal) << 1)));
        }
    };
}
