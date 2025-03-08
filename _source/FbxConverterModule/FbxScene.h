#pragma once

#include <map>
#include <string>
#include <vector>
#include "Base/BaseDefinition.h"

namespace Sparrow::Tools
{
    // Forward Declaration
    struct Node;
    struct SubMesh;
    struct Mesh;
    struct AxisBoundingBox;
    struct Material;
    struct Light;
    struct Camera;
    struct Texture;
    struct TextureSlot;
    struct Skeleton;
    struct BoneInfluence;
    struct VertexBoneData;
    struct Animation;
    struct BlendShape;
    struct BlendShapeAnimation;

    // Scene
    struct Scene
    {
        std::vector<Node>                 m_NodeList;
        std::vector<Mesh>                 m_MeshList;
        std::vector<Material>             m_MaterialList;
        std::vector<Texture>              m_TextureList;
        std::vector<Skeleton>             m_SkeletonList;
        std::vector<Animation>            m_AnimationList;
        std::vector<BlendShape>           m_BlendShapeList;
        std::vector<BlendShapeAnimation>  m_BlendShape;
    };

    // Node
    struct Node
    {
        std::string        m_Name;
        int                m_Index;
        Matrix4f           m_LocalTransform;
        Matrix4f           m_GlobalTransform;

        int              m_Parent;
        std::vector<int> m_Children;

        int m_MeshIndex;
    };

    // Mesh
    struct SubMesh
    {
        std::string        m_Name;
        int                m_Index;
        int                m_Start;
        int                m_End;
        int                m_MaterialIndex;
    };

    struct AxisBoundingBox
    {
        Vector3f min;
        Vector3f max;
        Vector3f center;
        Float    radius;
    };

    struct Mesh
    {
        std::string             m_Name;
        std::vector<int>        m_Indices;
        std::vector<Vector3f>   m_Positions;
        std::vector<Vector3f>   m_Normals;
        std::vector<Vector3f>   m_Tangents;
        std::vector<Vector3f>   m_Binormals;
        std::vector<Vector2f>   m_Texcoord0;
        std::vector<Vector2f>   m_Texcoord1;
        std::vector<int>        m_IsMirrors;

        std::vector<SubMesh>           m_SubMeshes;
        std::vector<VertexBoneData>    m_VertexBoneData;
        std::vector<std::string>       m_LocalSkeletons;

        AxisBoundingBox m_AxisBoundingBox;
    };

    struct TextureSlot
    {
        Int m_TextureIndex;
        Int m_TexcoordIndex;
    };

    struct Texture
    {
        std::string m_Path;
    };

    struct Material
    {
        std::string m_Name;
        std::string m_PBR;
        TextureSlot m_BaseColor;
        TextureSlot m_Normal;
        TextureSlot m_Specular;
        TextureSlot m_Ambient;
        TextureSlot m_Opacity;
        TextureSlot m_Emssive;
    };

    struct Skeleton
    {
        Int              m_SkeletonRootIndex;
        std::string      m_SkeletonRootName;
        std::vector<Int> m_BoneIndices;
    };

    // Skeleton
    struct BoneInfluence
    {
        std::string        m_Name;
        std::vector<int>   m_indices;
        std::vector<float> m_weights;
        std::vector<float> m_postion;
        std::vector<float> m_rotation_quaternion;
        std::vector<float> m_scale;
        std::vector<float> m_matrix_inverse_by_row;
    };

    struct VertexBoneData
    {
        int   bone_index[4];
        float bone_weight[4];
    };

    // Animation
    struct Vector3fKeyFrame
    {
        double   m_Time;
        int      m_Frame;
        Vector3f m_Value;
    };

    struct QuaternionKeyFrame
    {
        double   m_Time;
        int      m_Frame;
        Vector4f m_Value;
    };

    struct NodeAnimationChannel
    {
        std::string m_Name;

        std::vector<Vector3fKeyFrame>   m_PositionKeys;
        std::vector<QuaternionKeyFrame> m_RotationKeys;
        std::vector<Vector3fKeyFrame>   m_ScaleingKeys;
    };

    struct Animation
    {
        std::string                         m_Name;
        double                              m_DurationInTicks;
        int                                 m_TicksPerSeconds;
        std::vector<NodeAnimationChannel>   m_Channels;

        // TODO: 
        double start;
        double end;
        double duration;
    };

    // BlendShape
    struct BlendShapeChannel
    {
        std::string m_Name;
        int m_IndexCount;
        int m_IndexOffset;
        int m_VertexCount;
        int m_VertexOffset;

        std::vector<int>     m_Indices;
        std::vector<float>   m_Positions;
        std::vector<float>   m_Normals;
        std::vector<int>     m_IsNormalExist;
    };

    struct BlendShapeAnimationChannel
    {
        std::string          m_Name;
        std::map<int, float> m_KeyFrames;
    };

    struct BlendShapeAnimation
    {
        std::string                             m_Name;
        std::vector<BlendShapeAnimationChannel> m_Channels;
    };

    struct PrefetchInfo;
    struct MeshNodeInfo;
    struct MaterialNodeInfo;
    struct AnimationStackInfo;

    enum class UnitInfo;
    enum class CoordSystem;
    enum class AxisDirection;
    struct AxisInfo;

    struct MeshNodeInfo
    {
        std::string m_Name;
        size_t      m_MeshIndex;
        Bool        m_IsTriangulated;
    };

    struct MaterialNodeInfo
    {
        std::string m_Name;
        size_t      m_MaterialIndex;
        Bool        m_IsSpecificated;
    };

    struct AnimationStackInfo
    {
        std::string m_Name;
        size_t      m_AnimationStackIndex;
        size_t      m_AnimationLayerCount;        
    };

    enum class UnitInfo
    {
        mm,
        dm,
        cm,
        m,
        km,
        inch,
        foot,
        mile,
        yard,    
    };

    enum class CoordSystem
    {
        left_hand,
        right_hand,
    };

    enum class AxisDirection
    {
        positive_x,
        positive_z,
        positive_y,
        negative_x,
        negative_y,
        negative_z,
    };

    struct AxisInfo
    {
        CoordSystem m_CoordSystem;
        AxisDirection m_UpDirection;
    };

    struct PrefetchInfo
    {
        std::vector<MeshNodeInfo>       m_MeshNodeInfoList;
        std::vector<MaterialNodeInfo>   m_MaterialNodeInfoList;
        std::vector<AnimationStackInfo> m_AnimationStackInfoList;
        std::vector<std::string>        m_SkeletonNames;
        Bool                            m_HasSkeleton;
        UnitInfo                        m_Unit;
        AxisInfo                        m_Axis;
        std::string                     m_Creator;
    };
}