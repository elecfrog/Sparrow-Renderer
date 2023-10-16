#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Resource/Asset.h"
#include "Function/Animation/AnimClip.h"

/*
 * is actually a bone, a joint, every name is right to describe it.
 * My naming specfiction refers to https://raw.githubusercontent.com/KhronosGroup/glTF/main/specification/2.0/figures/gltfOverview-2.0.0b.png
 */

struct BoneNode
{
    std::string name;
    uint32_t    boneID = -1; // -1 if no parent
    BoneNode*   parent = nullptr;
    std::vector<BoneNode*> children;

    glm::mat4   offsetMatrix;
};

struct BoneInfo : Asset
{
    BoneNode* root = nullptr;
    std::unordered_map<std::string, BoneNode*>  nodes; // store all bone nodes references
    std::vector<std::shared_ptr<AnimationClip>> animations;
};
