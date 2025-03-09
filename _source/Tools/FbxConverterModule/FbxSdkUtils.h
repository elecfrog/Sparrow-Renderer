#pragma once

#include <fbxsdk.h>
#include "FbxScene.h"
#include "Core/Log/LogSystem.h"

namespace Sparrow::Tools
{
    struct FbxSdkObjects
    {
        FbxManager*           m_SdkManager           { nullptr };
        FbxScene*             m_SdkScene             { nullptr };
        FbxImporter*          m_SdkImporter          { nullptr };
        FbxExporter*          m_SdkExporter          { nullptr };
        FbxGeometryConverter* m_SdkGeometryConverter { nullptr };
    };

    class FbxSdkUtils
    {
    public:
        static FbxManager* CreateFbxManager();
        static FbxScene*   CreateFbxScene(FbxManager* sdk_manager);
        static Bool        LoadScene(FbxSdkObjects& sdk_objects, const char* fbx_path);
        static Bool        LoadFbxFile(FbxSdkObjects& sdk_objects, const std::string& fbx_path);
        static Bool        LoadFbxFile(FbxSdkObjects& sdk_objects, const FbxString& fbx_path);

        static void     GetSkeletonNodesNameInfo(FbxNode* root_node, std::vector<std::string>& skeleton_names);
        static FbxNode* GetRootSkeletonNode(FbxNode* fbx_node);
        static FbxNode* TryGetFirstRootBone(FbxNode* fbx_node);

        static void GetMeshNodesInfo(FbxScene* fbx_scene, std::vector<MeshNodeInfo>& mesh_node_info);
        static void GetMaterialNodesInfo(const FbxScene* fbx_scene, std::vector<MaterialNodeInfo>& material_node_info);
        static void GetAnimationTakesInfo(const FbxScene* fbx_scene, std::vector<AnimationStackInfo>& anim_stack_info);
        static void GetSkeletonInfo(const FbxScene* fbx_scene, bool& has_skeleton);
        static void GetUnitInfo(FbxScene* fbx_scene, UnitInfo& unit_info);
        static void GetAxisInfo(FbxScene* fbx_scene, AxisInfo& axis_info);
        static void PrefetchFbxData(FbxSdkObjects& sdk_objects, PrefetchInfo& prefetch);
        
        static Bool ExportFbxSceneToDisk(const std::string& fbx_path, FbxManager* sdk_manager, FbxScene* fbx_scene);
        static void CreateFbxDocument(FbxManager* sdk_manager, FbxScene* fbx_scene);
        
        static void  ConvertUnitSystemBySdk(FbxScene* fbx_scene);
        static Float ConvertUnitSystemByHand(FbxScene* fbx_scene);
        static void  ConvertMaxAxisSystem(FbxScene* fbx_scene);

        static FbxMesh* TriangulateMeshNode(FbxMesh* fbx_mesh, FbxGeometryConverter* fbx_geom_converter);

        static std::string NativeToUTF8(const std::string& str);

    private:
        static FbxString   GetSdkStringFromStdString(const std::string& fbx_path);
        static std::string GetStdStringFromSdkString(const char* fbx_path);
        static std::string GetStdStringFromSdkString(const FbxString& fbx_path);
    };
}
