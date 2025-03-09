#include "FbxSdkUtils.h"
#include <fbxsdk.h>
#include <fbxsdk/core/fbxmanager.h>
#include <fbxsdk/scene/fbxscene.h>
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/scene/geometry/fbxskin.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/fileio/fbxexporter.h>

#include <format>
#include <queue>
#include "Core/Log/LogSystem.h"

namespace Sparrow::Tools
{
    FbxManager* FbxSdkUtils::CreateFbxManager()
    {
        FbxManager* sdk_manager = FbxManager::Create();
        if (!sdk_manager)
        {
            LOG_ERROR(LogModule::Fbx, "FbxManager::CreateFbxManager failed.");
            return nullptr;
        }
        FbxIOSettings* ios = FbxIOSettings::Create(sdk_manager, IOSROOT);
        sdk_manager->SetIOSettings(ios);
        // Set the import states. By default, the import states are always set to
        // true. The code below shows how to change these states
        ios->SetBoolProp(IMP_RELAXED_FBX_CHECK, true);
        ios->SetBoolProp(IMP_FBX_MATERIAL, true);
        ios->SetBoolProp(IMP_FBX_TEXTURE, true);
        ios->SetBoolProp(IMP_FBX_LINK, true);
        ios->SetBoolProp(IMP_FBX_SHAPE, true);
        ios->SetBoolProp(IMP_FBX_GOBO, true);
        ios->SetBoolProp(IMP_FBX_ANIMATION, true);
        ios->SetBoolProp(IMP_SKINS, true);
        ios->SetBoolProp(IMP_DEFORMATION, true);
        ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
        ios->SetBoolProp(IMP_TAKE, true);
        return sdk_manager;
    }

    FbxScene* FbxSdkUtils::CreateFbxScene(FbxManager* sdk_manager)
    {
        RETURN_NULL_IF_NULL(sdk_manager);

        LOG_INFO(LogModule::Fbx, "Autodesk FBX SDK version: {}", sdk_manager->GetVersion());

        FbxScene* fbx_scene = FbxScene::Create(sdk_manager, "");
        if (!fbx_scene)
        {
            LOG_ERROR(LogModule::Fbx, "Unable to create FBX scene!");
            RETURN_NULL_IF_NULL(fbx_scene);
        }

        return fbx_scene;
    }

    Bool FbxSdkUtils::LoadScene(FbxSdkObjects& sdk_objects, const char* fbx_path)
    {
        // Get the file version number generate by the FBX SDK.
        int sdk_major_version, sdk_minor_version, sdk_revision_version;
        FbxManager::GetFileFormatVersion(sdk_major_version, sdk_minor_version, sdk_revision_version);
        LOG_INFO(LogModule::Fbx, "FBX SDK version: {}:{}:{}", sdk_major_version, sdk_minor_version, sdk_revision_version);

        // Create an importer.
        sdk_objects.m_SdkImporter = FbxImporter::Create(sdk_objects.m_SdkManager, "");

        // Initialize the importer by providing a filename.
        const bool import_status = sdk_objects.m_SdkImporter->Initialize(fbx_path, -1, sdk_objects.m_SdkManager->GetIOSettings());
        if (!sdk_objects.m_SdkImporter->GetStatus())
        {
            LOG_ERROR(LogModule::Fbx, "File Corrupted.");
            return false;
        }

        if (!import_status)
        {
            FbxString error = sdk_objects.m_SdkImporter->GetStatus().GetErrorString();
            LOG_ERROR(LogModule::Fbx, "Call to FbxImporter::Initialize() failed.");
            LOG_ERROR(LogModule::Fbx, "Cannot open FBX file: {}", fbx_path);
            return false;
        }

        int file_major_version, file_minor_version, file_revision_version;
        sdk_objects.m_SdkImporter->GetFileVersion(file_major_version, file_minor_version, file_revision_version);
        LOG_INFO(LogModule::Fbx, "FBX file format version for this FBX SDK is: {}:{}:{}",
                 file_major_version, file_minor_version, file_revision_version);

        // Import the scene.
        bool status = sdk_objects.m_SdkImporter->Import(sdk_objects.m_SdkScene);
        FbxStatus fbx_status = sdk_objects.m_SdkImporter->GetStatus();
        if (!status && fbx_status == FbxStatus::ePasswordError)
        {
            LOG_ERROR(LogModule::Fbx, "Need password");
            return false;
        }

        if (!status || (fbx_status != FbxStatus::eSuccess))
        {
            if (status)
            {
                LOG_WARN(LogModule::Fbx, "The importer was able to read the file but with errors!");
            }
            else
            {
                LOG_ERROR(LogModule::Fbx, "Importer failed to load the file!");
            }

            if (fbx_status != FbxStatus::eSuccess)
            {
                LOG_ERROR(LogModule::Fbx, "Last error message: {}", 
                    sdk_objects.m_SdkImporter->GetStatus().GetErrorString());
            }
        }

        return status;
    }

    Bool FbxSdkUtils::LoadFbxFile(FbxSdkObjects& sdk_objects, const std::string& fbx_path)
    {
        if (fbx_path.empty())
        {
            LOG_ERROR(LogModule::Fbx, "FBX file path is empty");
            return false;
        }

        sdk_objects.m_SdkManager = CreateFbxManager();
        sdk_objects.m_SdkScene = CreateFbxScene(sdk_objects.m_SdkManager);

        if (!LoadScene(sdk_objects, fbx_path.c_str()))
        {
            LOG_ERROR(LogModule::Fbx, "An error occurred while loading the scene");
            return false;
        }
        return true;
    }

    Bool FbxSdkUtils::LoadFbxFile(FbxSdkObjects& sdk_objects, const FbxString& fbx_path)
    {
        if (!fbx_path.Buffer())
        {
            LOG_ERROR(LogModule::Fbx, "FBX file path is empty");
            return false;
        }

        sdk_objects.m_SdkManager = CreateFbxManager();
        sdk_objects.m_SdkScene = CreateFbxScene(sdk_objects.m_SdkManager);

        if (!LoadScene(sdk_objects, fbx_path.Buffer()))
        {
            LOG_ERROR(LogModule::Fbx, "An error occurred while loading the scene");
            return false;
        }

        return true;
    }

    void FbxSdkUtils::GetSkeletonNodesNameInfo(FbxNode* root_node, std::vector<std::string>& skeleton_names)
    {
        RETURN_IF_NULL(root_node);

        root_node = TryGetFirstRootBone(root_node);
        RETURN_IF_NULL(root_node);

        std::queue<FbxNode*> node_queue;
        node_queue.push(root_node);
        while (!node_queue.empty())
        {
            FbxNode* node = node_queue.front();
            if (node)
            {
                skeleton_names.push_back(node->GetName());
                for (int i = 0; i < node->GetChildCount(); i++)
                {
                    node_queue.push(node->GetChild(i));
                }
            }
            node_queue.pop();
        }
    }

    FbxNode* FbxSdkUtils::GetRootSkeletonNode(FbxNode* fbx_node)
    {
        RETURN_NULL_IF_NULL(fbx_node);
        auto parent_node = fbx_node->GetParent();

        while (fbx_node && parent_node)
        {
            FbxNodeAttribute* attribute = parent_node->GetNodeAttribute();
            if (!attribute)
            {
                break;
            }

            FbxNodeAttribute::EType attribute_type = attribute->GetAttributeType();
            Bool is_mesh = attribute_type == FbxNodeAttribute::eMesh;
            Bool is_skeleton = attribute_type == FbxNodeAttribute::eSkeleton;
            Bool is_null = attribute_type == FbxNodeAttribute::eNull;
            Bool is_blender_armature_bone = false;
            if (is_mesh || is_skeleton || (is_null && !is_blender_armature_bone))
            {
                if (is_mesh)
                {
                    FbxMesh* mesh = fbx_node->GetMesh();
                    if (mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
                    {
                        break;
                    }
                }

                fbx_node = parent_node;
                parent_node = fbx_node->GetParent();
            }
        }
        return fbx_node;
    }

    FbxNode* FbxSdkUtils::TryGetFirstRootBone(FbxNode* fbx_node)
    {
        std::queue<FbxNode*> node_queue;
        node_queue.push(fbx_node);
        while (!node_queue.empty())
        {
            FbxNode* current_node = node_queue.front();
            node_queue.pop();
            if (current_node == nullptr)
            {
                continue;
            }
            for (int child_idx = 0; child_idx < current_node->GetChildCount(); child_idx++)
            {
                node_queue.push(current_node->GetChild(child_idx));
            }
            FbxMesh* fbx_mesh = current_node->GetMesh();
            if (fbx_mesh == nullptr || fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) <= 0)
            {
                continue;
            }

            // find root bone
            FbxSkin* deformer = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(0, FbxDeformer::eSkin));
            if (!deformer)
            {
                continue;
            }
            int cluster_count = deformer->GetClusterCount();
            for (int cluster_idx = 0; cluster_idx < cluster_count; ++cluster_idx)
            {
                FbxNode* root_bone_link = deformer->GetCluster(cluster_idx)->GetLink(); // Get the bone influences by this first cluster
                root_bone_link = GetRootSkeletonNode(root_bone_link);                   // Get the skeleton root itself

                if (root_bone_link)
                {
                    return root_bone_link;
                }
            }
        }
        return nullptr;
    }

    void FbxSdkUtils::GetMeshNodesInfo(FbxScene* fbx_scene, std::vector<MeshNodeInfo>& mesh_node_info)
    {
        // Find all mesh nodes
        int mesh_count = fbx_scene->GetSrcObjectCount(FbxCriteria::ObjectType(FbxMesh::ClassId));
        for (int idx = 0; idx < mesh_count; ++idx)
        {
            MeshNodeInfo curr_mesh_node_info;
            curr_mesh_node_info.m_MeshIndex = idx;

            FbxMesh* fbx_mesh = (FbxMesh*)fbx_scene->GetSrcObject(FbxCriteria::ObjectType(FbxMesh::ClassId), idx);
            if (!fbx_mesh)
            {
                continue;
            }

            curr_mesh_node_info.m_Name = fbx_mesh->GetName();
            curr_mesh_node_info.m_IsTriangulated = fbx_mesh->IsTriangleMesh();
            mesh_node_info.emplace_back(std::move(curr_mesh_node_info));
        }
    }

    void FbxSdkUtils::GetMaterialNodesInfo(const FbxScene* fbx_scene, std::vector<MaterialNodeInfo>& material_node_info)
    {
        // Find all material nodes
        int material_count = fbx_scene->GetSrcObjectCount<FbxSurfaceMaterial>();
        for (int idx = 0; idx < material_count; ++idx)
        {
            MaterialNodeInfo curr_material_node_info;
            curr_material_node_info.m_MaterialIndex = idx;
            FbxSurfaceMaterial* fbx_material = fbx_scene->GetSrcObject<FbxSurfaceMaterial>(idx);
            if (fbx_material)
            {
                continue;
            }
            curr_material_node_info.m_Name = fbx_material->GetName();
            curr_material_node_info.m_IsSpecificated = false;
            material_node_info.emplace_back(std::move(curr_material_node_info));
        }
    }

    void FbxSdkUtils::GetAnimationTakesInfo(const FbxScene* fbx_scene, std::vector<AnimationStackInfo>& anim_stack_info)
    {
        // Find all animation stacks
        int anim_stack_count = fbx_scene->GetSrcObjectCount<FbxAnimStack>();
        for (int idx = 0; idx < anim_stack_count; ++idx)
        {
            AnimationStackInfo curr_anim_stack_info;
            curr_anim_stack_info.m_AnimationStackIndex = idx;
            FbxAnimStack* fbx_anim_stack = fbx_scene->GetSrcObject<FbxAnimStack>(idx);
            if (!fbx_anim_stack)
            {
                continue;
            }
            curr_anim_stack_info.m_Name = fbx_anim_stack->GetName();
            curr_anim_stack_info.m_AnimationLayerCount = fbx_anim_stack->GetMemberCount<FbxAnimLayer>();
            anim_stack_info.emplace_back(std::move(curr_anim_stack_info));
        }
    }

    void FbxSdkUtils::GetSkeletonInfo(const FbxScene* fbx_scene, bool& has_skeleton)
    {
        int skeleton_count = fbx_scene->GetSrcObjectCount<FbxSkeleton>();
        has_skeleton = skeleton_count > 0;
    }

    void FbxSdkUtils::GetUnitInfo(FbxScene* fbx_scene, UnitInfo& unit_info)
    {
        // Find all animation stacks
        FbxSystemUnit global_unit = fbx_scene->GetGlobalSettings().GetSystemUnit();
        if (global_unit == FbxSystemUnit::m)
        {
            unit_info = UnitInfo::m;
        }
        if (global_unit == FbxSystemUnit::cm)
        {
            unit_info = UnitInfo::cm;
        }
        if (global_unit == FbxSystemUnit::mm)
        {
            unit_info = UnitInfo::mm;
        }
        if (global_unit == FbxSystemUnit::dm)
        {
            unit_info = UnitInfo::dm;
        }
        if (global_unit == FbxSystemUnit::km)
        {
            unit_info = UnitInfo::km;
        }
        if (global_unit == FbxSystemUnit::Inch)
        {
            unit_info = UnitInfo::inch;
        }
        if (global_unit == FbxSystemUnit::Foot)
        {
            unit_info = UnitInfo::foot;
        }
        if (global_unit == FbxSystemUnit::Mile)
        {
            unit_info = UnitInfo::mile;
        }
        if (global_unit == FbxSystemUnit::Yard)
        {
            unit_info = UnitInfo::yard;
        }
    }

    void FbxSdkUtils::GetAxisInfo(FbxScene* fbx_scene, AxisInfo& axis_info)
    {
        using FbxUpVector = FbxAxisSystem::EUpVector;
        using FbxFrontVector = FbxAxisSystem::EFrontVector;
        using FbxCoordSystem = FbxAxisSystem::ECoordSystem;
        FbxAxisSystem global_axis = fbx_scene->GetGlobalSettings().GetAxisSystem();

        int up_vector_sign = 1;
        FbxUpVector up_vector = global_axis.GetUpVector(up_vector_sign);
        if (up_vector == FbxUpVector::eXAxis && up_vector_sign == 1)
        {
            axis_info.m_UpDirection = AxisDirection::positive_x;
        }
        else if (up_vector == FbxUpVector::eXAxis && up_vector_sign == -1)
        {
            axis_info.m_UpDirection = AxisDirection::negative_x;
        }
        else if (up_vector == FbxUpVector::eYAxis && up_vector_sign == 1)
        {
            axis_info.m_UpDirection = AxisDirection::positive_y;
        }
        else if (up_vector == FbxUpVector::eYAxis && up_vector_sign == -1)
        {
            axis_info.m_UpDirection = AxisDirection::negative_y;
        }
        else if (up_vector == FbxUpVector::eZAxis && up_vector_sign == 1)
        {
            axis_info.m_UpDirection = AxisDirection::positive_z;
        }
        else if (up_vector == FbxUpVector::eZAxis && up_vector_sign == -1)
        {
            axis_info.m_UpDirection = AxisDirection::negative_z;
        }

        FbxCoordSystem coord_system = global_axis.GetCoorSystem();
        if (coord_system == FbxCoordSystem::eLeftHanded)
        {
            axis_info.m_CoordSystem = CoordSystem::left_hand;
        }
        else
        {
            axis_info.m_CoordSystem = CoordSystem::right_hand;
        }
    }

    void FbxSdkUtils::PrefetchFbxData(FbxSdkObjects& sdk_objects, PrefetchInfo& prefetch)
    {
        auto& fbx_scene = sdk_objects.m_SdkScene;
        // Find all mesh nodes
        GetMeshNodesInfo(fbx_scene, prefetch.m_MeshNodeInfoList);
        // Find all material nodes
        GetMaterialNodesInfo(fbx_scene, prefetch.m_MaterialNodeInfoList);
        // Find all animation stacks
        GetSkeletonNodesNameInfo(fbx_scene->GetRootNode(), prefetch.m_SkeletonNames);
        GetAnimationTakesInfo(fbx_scene, prefetch.m_AnimationStackInfoList);
        GetUnitInfo(fbx_scene, prefetch.m_Unit);
        GetAxisInfo(fbx_scene, prefetch.m_Axis);
        GetSkeletonInfo(fbx_scene, prefetch.m_HasSkeleton);
        auto& importer = sdk_objects.m_SdkImporter;
        auto* scene_info = importer->GetSceneInfo();
        if (!scene_info)
        {
            prefetch.m_Creator = "Unkown";
        }
        else
        {
            prefetch.m_Creator = std::string(scene_info->LastSaved_ApplicationName.Get().Buffer());
        }
    }

    bool FbxSdkUtils::ExportFbxSceneToDisk(const std::string& fbx_path, FbxManager* sdk_manager, FbxScene* fbx_scene)
    {
        // Create an exporter
        FbxExporter* fbx_exporter = FbxExporter::Create(sdk_manager, "");
        int file_format = -1;
        FbxIOPluginRegistry* io_plugin_registry = sdk_manager->GetIOPluginRegistry();
        if (file_format < 0 || file_format >= io_plugin_registry->GetWriterFormatCount())
        {
            // Write in fall back format in less no ASCII format found
            file_format = io_plugin_registry->GetNativeWriterFormat();
            int format_idx, format_count = io_plugin_registry->GetWriterFormatCount();
            for (format_idx = 0; format_idx < format_count; ++format_idx)
            {
                if (io_plugin_registry->WriterIsFBX(format_idx))
                {
                    FbxString lDesc = io_plugin_registry->GetWriterFormatDescription(format_idx);
                    const char* lDescStr = "binary";
                    if (lDesc.Find(lDescStr) >= 0)
                    {
                        file_format = format_idx;
                        break;
                    }
                }
            }
        }

        // Set the export states
        FbxIOSettings* fbx_io_setting = sdk_manager->GetIOSettings();
        fbx_io_setting->SetBoolProp(EXP_FBX_MATERIAL, true);
        fbx_io_setting->SetBoolProp(EXP_FBX_TEXTURE, true);
        fbx_io_setting->SetBoolProp(EXP_FBX_EMBEDDED, true);
        fbx_io_setting->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

        // Initialize the exporter by providing a filename
        if (fbx_exporter->Initialize(fbx_path.c_str(), file_format, sdk_manager->GetIOSettings()) == false)
        {
            LOG_ERROR(LogModule::Fbx, "Call to FbxExporter:Initialize() failed.");
            LOG_ERROR(LogModule::Fbx, "Error: {}", fbx_exporter->GetStatus().GetErrorString());
            return false;
        }

        // Export the scene
        fbx_exporter->Export(fbx_scene);
        // Destroy the exporter
        fbx_exporter->Destroy();

        return true;
    }

    void FbxSdkUtils::CreateFbxDocument(FbxManager* sdk_manager, FbxScene* fbx_scene)
    {
        // create scene info
        FbxDocumentInfo* fbx_scene_info = FbxDocumentInfo::Create(sdk_manager, "SceneInfo");
        fbx_scene_info->mTitle = "Scene";
        fbx_scene_info->mSubject = "Exported Fbx File";
        fbx_scene_info->mAuthor = "Elecfrog";
        fbx_scene_info->Original_ApplicationVendor.Set("Sparrow Engine");
        fbx_scene_info->Original_ApplicationName.Set("Sparrow Engine");
        fbx_scene_info->Original_ApplicationVersion.Set("1.0.0");
        fbx_scene_info->LastSaved_ApplicationVendor.Set("Sparrow Engine");
        fbx_scene_info->LastSaved_ApplicationName.Set("Sparrow Engine");
        fbx_scene_info->LastSaved_ApplicationVersion.Set("1.0.0");
        fbx_scene->SetSceneInfo(fbx_scene_info);

        // chaos setting
        auto& global_setting = fbx_scene->GetGlobalSettings();

        global_setting.SetAxisSystem(FbxAxisSystem::EPreDefinedAxisSystem::eMax);
        global_setting.SetOriginalUpAxis(FbxAxisSystem::EPreDefinedAxisSystem::eMax);

        global_setting.SetOriginalSystemUnit(FbxSystemUnit::m);

        global_setting.SetSystemUnit(FbxSystemUnit::m);
        global_setting.SetTimeMode(FbxTime::eFrames30);
    }

    void FbxSdkUtils::ConvertUnitSystemBySdk(FbxScene* fbx_scene)
    {
        // process unit system
        if (fbx_scene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::m)
        {
            constexpr FbxSystemUnit::ConversionOptions conversion_options = {
                false, /* mConvertRrsNodes */
                true, /* mConvertAllLimits */
                true, /* mConvertClusters */
                true, /* mConvertLightIntensity */
                true, /* mConvertPhotometricLProperties */
                true /* mConvertCameraClipPlanes */
            };
            // Convert the scene to meters using the defined options.
            FbxSystemUnit::m.ConvertScene(fbx_scene, conversion_options);
        }
    }

    Float FbxSdkUtils::ConvertUnitSystemByHand(FbxScene* fbx_scene)
    {
        UnitInfo unit_info{};
        GetUnitInfo(fbx_scene, unit_info);
        float mesh_unit_scale = 1.0f;
        switch (unit_info)
        {
        case UnitInfo::m:
            mesh_unit_scale *= 1.0f;
            break;
        case UnitInfo::cm:
            mesh_unit_scale *= 0.01f;
            break;
        case UnitInfo::inch:
            mesh_unit_scale *= 0.0254f;
            break;
        case UnitInfo::foot:
            mesh_unit_scale *= 0.3048f;
            break;
        case UnitInfo::mm:
            mesh_unit_scale *= 0.001f;
            break;
        }

        return mesh_unit_scale;
    }

    void FbxSdkUtils::ConvertMaxAxisSystem(FbxScene* fbx_scene)
    {
        FbxAxisSystem::Max.ConvertScene(fbx_scene);
    }

    FbxMesh* FbxSdkUtils::TriangulateMeshNode(FbxMesh* fbx_mesh, FbxGeometryConverter* fbx_geom_converter)
    {
        if (fbx_mesh->IsTriangleMesh()) { return fbx_mesh; }

        FbxNodeAttribute* attribute = fbx_geom_converter->Triangulate(fbx_mesh, true);
        if (attribute && attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
        {
            fbx_mesh = (FbxMesh*)attribute;
            return fbx_mesh;
        }

        return nullptr;
    }

    std::string FbxSdkUtils::NativeToUTF8(const std::string& str) {
#if _WIN32
        char* u8cstr = nullptr;
#if (_UNICODE || UNICODE)
        FbxWCToUTF8(reinterpret_cast<const char*>(str.c_str()), u8cstr);
#else
        FbxAnsiToUTF8(str.c_str(), u8cstr);
#endif
        if (!u8cstr) {
            return str;
        }
        else {
            std::string u8str = u8cstr;
            delete[] u8cstr;
            return u8str;
        }
#else
        return str;
#endif
    }

    FbxString FbxSdkUtils::GetSdkStringFromStdString(const std::string& fbx_path)
    {
        char* newPathStr {nullptr};
        FbxString new_path_str = "";
        FbxAnsiToUTF8(fbx_path.c_str(), newPathStr);
        new_path_str = newPathStr;
        return new_path_str;
    }

    std::string FbxSdkUtils::GetStdStringFromSdkString(const char* fbx_path)
    {
        std::string ret = "";
        char* new_str = nullptr;
        FbxUTF8ToAnsi(fbx_path, new_str);
        if (new_str)
        {
            ret = new_str;
        }
        return ret;
    }

    std::string FbxSdkUtils::GetStdStringFromSdkString(const FbxString& fbx_path)
    {
        std::string ret = "";
        char* new_str = nullptr;
        FbxUTF8ToAnsi(fbx_path.Buffer(), new_str);
        if (new_str)
        {
            ret = new_str;
        }
        return ret;
    }
}
