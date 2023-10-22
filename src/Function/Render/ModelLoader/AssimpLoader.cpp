#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Utils/Utility.hpp"
#include "Platform/FileSystem/FileSystem.h"
#include "Function/Render/Model.h"
#include "Function/Render/Material.h"
#include "Function/Render/Skin.h"
#include "Resource/ResourceManager.h"
#include "Function/Animation/AnimClip.h"

namespace glm
{
inline glm::vec3 toVec3(const aiVector3D &_val) {
    return glm::vec3(_val.x, _val.y, _val.z);
}

inline glm::vec4 toVec4(const aiColor4D &_val) {
    return glm::vec4(_val.r, _val.g, _val.b, _val.a);
}

inline glm::quat toQuat(const aiQuaternion &_val) {
    return glm::quat(glm::vec4(_val.x, _val.y, _val.z, _val.w));
}

inline glm::vec3 quatToEuler(const glm::quat &_quat) {
    return glm::eulerAngles(_quat);
}

// `mat` is an `aiMatrix4x4`
inline glm::mat4 toMat4(const aiMatrix4x4 &mat) {
    glm::mat4 result = glm::mat4(1.f);
    result[0][0] = mat.a1;
    result[1][0] = mat.a2;
    result[2][0] = mat.a3;
    result[3][0] = mat.a4;
    result[0][1] = mat.b1;
    result[1][1] = mat.b2;
    result[2][1] = mat.b3;
    result[3][1] = mat.b4;
    result[0][2] = mat.c1;
    result[1][2] = mat.c2;
    result[2][2] = mat.c3;
    result[3][2] = mat.c4;
    result[0][3] = mat.d1;
    result[1][3] = mat.d2;
    result[2][3] = mat.d3;
    result[3][3] = mat.d4;
    return result;
}
}


std::shared_ptr<Material> LoadMaterialByAssimp(Model *model, aiMaterial *material) {
    std::shared_ptr<Material> tmpMaterial = std::make_shared<Material>();
    tmpMaterial->m_ID = 0;
    tmpMaterial->m_Type = AssetType::MATERIAL;
    tmpMaterial->m_FileMetaInfo.name = material->GetName().C_Str();
    tmpMaterial->m_FileMetaInfo.fullPath = model->m_FileMetaInfo.fullPath;

    MaterialProperties tmpProp;
    /* Set Material Name */
    tmpProp.name = material->GetName().C_Str();

    // Read material properties
    aiColor4D diffuseColor;
    aiColor4D specularColor;
    aiColor4D ambientColor;
    aiColor4D emissiveColor;
    aiColor4D transportColor;
    aiColor4D refectiveColor;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
    material->Get(AI_MATKEY_COLOR_TRANSPARENT, transportColor);
    material->Get(AI_MATKEY_COLOR_REFLECTIVE, refectiveColor);
    tmpProp.diffuseColor = glm::toVec4(diffuseColor);
    tmpProp.specularColor = glm::toVec4(specularColor);
    tmpProp.ambientColor = glm::toVec4(ambientColor);
    tmpProp.emissiveColor = glm::toVec4(emissiveColor);
    tmpProp.transportColor = glm::toVec4(transportColor);
    tmpProp.refectiveColor = glm::toVec4(refectiveColor);

    material->Get(AI_MATKEY_OPACITY, tmpProp.opacity);
    material->Get(AI_MATKEY_TRANSPARENCYFACTOR, tmpProp.transparentFactor);
    material->Get(AI_MATKEY_BUMPSCALING, tmpProp.bumpScaling);
    material->Get(AI_MATKEY_SHININESS, tmpProp.shininess);
    material->Get(AI_MATKEY_REFLECTIVITY, tmpProp.refelectivity);
    material->Get(AI_MATKEY_SHININESS_STRENGTH, tmpProp.shininessStrength);
    material->Get(AI_MATKEY_REFRACTI, tmpProp.refracti);

    // ---------------------------------------------------------------------------
    // PBR material support
    // --------------------

    /* PBR Metallic/Roughness Workflow */
    material->Get(AI_MATKEY_BASE_COLOR, tmpProp.metallicRoughnessFactors.baseColorFactor);
    material->Get(AI_MATKEY_METALLIC_FACTOR, tmpProp.metallicRoughnessFactors.metallicFactor);
    material->Get(AI_MATKEY_ROUGHNESS_FACTOR, tmpProp.metallicRoughnessFactors.roughnessFactor);
    material->Get(AI_MATKEY_ANISOTROPY_FACTOR, tmpProp.metallicRoughnessFactors.anisotropyFactor);

    /* PBR Specular/Glossiness Workflow */
    material->Get(AI_MATKEY_SPECULAR_FACTOR, tmpProp.specularGlossinessFactors.specularFactor);
    material->Get(AI_MATKEY_GLOSSINESS_FACTOR, tmpProp.specularGlossinessFactors.glossinessFactor);

    /* Sheen */
    material->Get(AI_MATKEY_SHEEN_COLOR_FACTOR, tmpProp.sheenFactors.sheenFactor);
    material->Get(AI_MATKEY_SHEEN_ROUGHNESS_FACTOR, tmpProp.sheenFactors.sheenRoughnessFactor);

    /* Clearcoat */
    material->Get(AI_MATKEY_CLEARCOAT_FACTOR, tmpProp.clearCoatFactors.clearcoatFactor);
    material->Get(AI_MATKEY_CLEARCOAT_ROUGHNESS_FACTOR, tmpProp.clearCoatFactors.clearcoatRoughnessFactor);

    /* Transmission */
    material->Get(AI_MATKEY_TRANSMISSION_FACTOR, tmpProp.transmissionFactor);

    /* Volume */
    material->Get(AI_MATKEY_VOLUME_THICKNESS_FACTOR, tmpProp.volumeFactors.volumeThicknessFactor);
    material->Get(AI_MATKEY_VOLUME_ATTENUATION_DISTANCE, tmpProp.volumeFactors.volumeAttenuationDistance);

    /* Emissive */
    material->Get(AI_MATKEY_EMISSIVE_INTENSITY, tmpProp.emissiveIntensity);

    // Iterate through the texture slots of the material
    const auto &basepath = model->m_FileMetaInfo.fullPath;
    for (unsigned int j = 0; j < AI_TEXTURE_TYPE_MAX; j++) {
        const auto textureType = static_cast<aiTextureType>(j);
        const size_t textureCount = material->GetTextureCount(textureType);

        // Iterate through the textures of the current slot
        for (unsigned int k = 0; k < textureCount; k++) {
            ResourceManager rm;
            aiString texturePath;
            material->GetTexture(textureType, k, &texturePath);
            std::string str = FileSystem::JoinFileRoute(basepath, texturePath.C_Str());
            if (j == aiTextureType_BASE_COLOR || j == aiTextureType_DIFFUSE)
                tmpProp.cmTextures.albedo = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_NORMALS)
                tmpProp.cmTextures.normal = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_EMISSIVE)
                tmpProp.cmTextures.emissive = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_SPECULAR)
                tmpProp.sgTextures.specularGlossiness = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_AMBIENT_OCCLUSION || j == aiTextureType_AMBIENT || j == aiTextureType_LIGHTMAP)
                tmpProp.sgTextures.ao = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_METALNESS)
                tmpProp.mrTextures.metallic = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
            else if (j == aiTextureType_DIFFUSE_ROUGHNESS)
                tmpProp.mrTextures.roughness = std::static_pointer_cast<Texture2D>(rm.LoadResource(str));
        }
    }

    if (tmpProp.sgTextures.specularGlossiness != nullptr) {
        tmpProp.workFlow = PBRWorkFlow::PBR_WORKFLOW_SG;
    } else {
        tmpProp.workFlow = PBRWorkFlow::PBR_WORKFLOW_MR;
    }

    tmpMaterial->SetMaterialProperites(std::move(tmpProp));

    return tmpMaterial;
}

void TraverseNodeHierarchy(aiNode *node, BoneNode *parentBoneNode, std::shared_ptr<BoneInfo> boneInfo) {
    std::string nodeName = node->mName.C_Str();
    auto boneNodeIter = boneInfo->nodes.find(nodeName);
    if (boneNodeIter != boneInfo->nodes.end()) {
        BoneNode *boneNode = boneNodeIter->second;
        boneNode->parent = parentBoneNode;
        if (parentBoneNode) {
            parentBoneNode->children.push_back(boneNode);
        }
        parentBoneNode = boneNode;
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        TraverseNodeHierarchy(node->mChildren[i], parentBoneNode, boneInfo);
    }
}

std::shared_ptr<BoneInfo> ProcessBonesHiarchy(Model *model, aiNode *node, const aiScene *scene) {
    std::shared_ptr<BoneInfo> boneInfo = std::make_shared<BoneInfo>();

    uint32_t totalBoneCount = 0; // �������й��������ı���

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumBones; ++j) {
            aiBone *bone = mesh->mBones[j];
            std::string boneName = bone->mName.C_Str();
            BoneNode *boneNode = new BoneNode(); // �����µ�BoneNode
            boneInfo->nodes[boneName] = boneNode; // ���µ�BoneNode��ӵ�nodesӳ����
            boneNode->name = boneName;
            boneNode->boneID = totalBoneCount++;
            boneNode->offsetMatrix = glm::toMat4(bone->mOffsetMatrix);
        }
    }

    TraverseNodeHierarchy(scene->mRootNode, nullptr, boneInfo);

    return boneInfo;
}

std::shared_ptr<Mesh> ProcessSkinnedMeshNode(Model *model, aiMesh *mesh, const aiScene *scene) {
    std::shared_ptr<Mesh> tmpMesh = std::make_shared<Mesh>();

    std::vector<SkinnedVertex> skinnedVertices;
    skinnedVertices.resize(mesh->mNumVertices);

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // positions
        skinnedVertices[i].position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // normals
        skinnedVertices[i].normal = mesh->HasNormals()
                                    ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                                    : glm::vec3();

        // texture coordinates
        const bool uv_exist = mesh->mTextureCoords[0];
        skinnedVertices[i].texCoords = uv_exist
                                       ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                                       : glm::vec2();

        // tangents
        skinnedVertices[i].tangent = uv_exist
                                     ? glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z)
                                     : glm::vec3();
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const auto &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            tmpMesh->m_Indices.emplace_back(face.mIndices[j]);
    }


    // Vertex [BoneID, weight] Pair implementation
    std::vector<uint32_t> boneIndexVector(skinnedVertices.size(), 0);
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        const aiBone *bone = mesh->mBones[i];

        // Weights
        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            const auto &vertexID = bone->mWeights[j].mVertexId;
            const auto &weight = bone->mWeights[j].mWeight;
            const auto &slot = boneIndexVector[vertexID];

            if (weight > 0 && weight < 1) {
                skinnedVertices[vertexID].boneIDs[slot] = (float) model->m_BoneInfo->nodes[bone->mName.C_Str()]->boneID;
                skinnedVertices[vertexID].weights[slot] = weight;
                boneIndexVector[vertexID]++;
            }
        }
    }
    tmpMesh->m_SkinnedVertices = std::move(skinnedVertices);

    if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
        tmpMesh->m_Material = LoadMaterialByAssimp(model, scene->mMaterials[mesh->mMaterialIndex]);

    return tmpMesh;
}

std::shared_ptr<Mesh> ProcessMeshNode(Model *model, aiMesh *mesh, const aiScene *scene) {
    std::shared_ptr<Mesh> tmpMesh = std::make_shared<Mesh>();

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // positions
        glm::vec3 position{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        // normals
        glm::vec3 normal = mesh->HasNormals()
                           ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                           : glm::vec3();

        // texture coordinates
        bool uv_exist = mesh->mTextureCoords[0];
        glm::vec2 uv = uv_exist
                       ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                       : glm::vec2();

        // tangents
        glm::vec3 tangent = uv_exist
                            ? glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z)
                            : glm::vec3();

        tmpMesh->m_Vertices.emplace_back(AttribVertex{position, normal, tangent, uv});
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const auto &face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            tmpMesh->m_Indices.emplace_back(face.mIndices[j]);
    }

    // Materials
    if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
        tmpMesh->m_Material = LoadMaterialByAssimp(model, scene->mMaterials[mesh->mMaterialIndex]);

    return tmpMesh;
}

void ProcessNodes(bool hasAnimation, Model *model, aiNode *node, const aiScene *scene) {
    if (hasAnimation) {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            model->m_Meshes.emplace_back(ProcessSkinnedMeshNode(model, mesh, scene));
        }
    } else {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            model->m_Meshes.emplace_back(ProcessMeshNode(model, mesh, scene));
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNodes(hasAnimation, model, node->mChildren[i], scene);
    }
}

[[nodiscard]] std::shared_ptr<AnimationClip> ProcessAnimationNode(aiAnimation *animation, const aiScene *scene) {
    std::shared_ptr<AnimationClip> tmp = std::make_shared<AnimationClip>();

    tmp->name = animation->mName.C_Str();
    tmp->duration = animation->mDuration;
    tmp->FPS = animation->mTicksPerSecond;
    tmp->frameCount = animation->mChannels[0]->mNumPositionKeys;
    tmp->nodeAnimations.resize(animation->mNumChannels);

    // Iterate over all channels in the animation
    for (unsigned int j = 0; j < animation->mNumChannels; j++) {
        const aiNodeAnim *channel = animation->mChannels[j];

        tmp->nodeAnimations[j].nodeName = channel->mNodeName.C_Str();

        // Iterate over all position keyframes in the channel
        for (unsigned int k = 0; k < channel->mNumPositionKeys; k++) {
            const auto time = channel->mPositionKeys[k].mTime;
            const auto pos = glm::toVec3(channel->mPositionKeys[k].mValue);
            const auto rot = glm::toQuat(channel->mRotationKeys[k].mValue);
            const auto scaling = glm::toVec3(channel->mScalingKeys[k].mValue);
            tmp->nodeAnimations[j].keyFrames.emplace_back(KeyFrame{time, pos, rot, scaling});
        }
        if (channel->mNumPositionKeys > tmp->frameCount)
            tmp->frameCount = channel->mNumPositionKeys;
    }
    return tmp;
}

void ProcessAnimationClips(Model *model, aiNode *node, const aiScene *scene) {
    // Iterate over all animtion clips
    for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation *animation = scene->mAnimations[i];

        model->m_BoneInfo->animations.emplace_back(ProcessAnimationNode(animation, scene));
    }
}


std::unique_ptr<Model> Model::LoadModel(const std::string &filename) {
    return LoadByAssimp(filename);
}

std::unique_ptr<Model> Model::LoadModel(const std::filesystem::path &file_path) {
    return LoadByAssimp(file_path.string());
}


std::unique_ptr<Model> Model::LoadByAssimp(const std::string &filename) {
    std::unique_ptr<Model> model = std::make_unique<Model>();

    // Create an instance of the Assimp importer
    const FilePath filepath = filename.c_str();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate
                                                               | aiProcess_GenSmoothNormals
                                                               | aiProcess_FlipUVs
                                                               | aiProcess_CalcTangentSpace
                                                               | aiProcess_PopulateArmatureData);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    model->m_Type = AssetType::MODEL;
    model->m_FileMetaInfo = FileMetaInfo{filepath.filename().string(), filepath.parent_path().string()};

    const bool hasAnimations = scene->HasAnimations();
    if (hasAnimations) {
        model->m_BoneInfo = ProcessBonesHiarchy(model.get(), scene->mRootNode, scene);
        ProcessAnimationClips(model.get(), scene->mRootNode, scene);
    }
    ProcessNodes(hasAnimations, model.get(), scene->mRootNode, scene);

    return model;
}
