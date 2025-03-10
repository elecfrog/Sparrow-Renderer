#pragma once


#include <Core/Core.h>
#include "Animation/AnimClip.h"


namespace Sparrow
{
    /*
     * is actually a bone, a joint, every name is right to describe it.
     * My naming specfiction refers to https://raw.githubusercontent.com/KhronosGroup/glTF/main/specification/2.0/figures/gltfOverview-2.0.0b.png
     */

    struct BoneNode
    {
        std::string name;
        uint32_t boneID = -1; // -1 if no parent
        BoneNode* parent = nullptr;
        std::vector<BoneNode*> children;

        glm::mat4 offsetMatrix;
    };

    struct BoneInfo : Sparrow::Asset
    {
        BoneNode* root = nullptr;
        std::unordered_map<std::string, BoneNode*> nodes; // store all bone nodes references
        std::vector<std::shared_ptr<Animation>> animations;
    };
}
