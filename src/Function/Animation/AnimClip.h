#pragma once

struct KeyFrame
{
    double time;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 sacling;
};

struct AnimationNode
{
    std::string nodeName;
    std::vector<KeyFrame> keyFrames;
};

struct AnimationClip : Asset
{
    std::string name;
    double duration;
    double FPS;
    uint32_t frameCount;
    std::vector<AnimationNode> nodeAnimations;
};



