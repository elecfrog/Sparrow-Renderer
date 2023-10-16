/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <optional>
#include "Platform/FileSystem/FileSystem.h"

struct Cone;

namespace elf::anim {
    // Skeleton Data
    struct BVHSkeleton;
    struct BVHJoint;
    struct BVHMotionData;
    struct BVHMotionFrame;
    struct BVHRawData;
    struct BonePositions;

    struct BVHSkeleton {
        std::shared_ptr<BVHJoint> dummyRoot;
        std::vector<std::shared_ptr<BVHJoint>> joints;
    };

    struct BVHJoint {
        std::string name;
        std::uint16_t channels;
        glm::vec3 globalLocation; // XYZ
        glm::vec3 translation; // XYZ
        glm::vec3 rotation; // ZYX
        bool isEnd;
        std::optional<glm::vec3> endSiteOffset;

        std::shared_ptr<BVHJoint> parent;
        std::vector<std::shared_ptr<BVHJoint>> childrens;

        glm::mat4 rotationMatrix = glm::mat4(1.f);
        glm::mat4 globalRotationMatrix = glm::mat4(1.f);
    };

    // Motion Data
    struct BVHMotionData {
        std::uint32_t frames;
        double frameTime;
        std::uint16_t boneNumber;
        std::vector<double> rawData;
        // each line of bvh motion data 
        std::vector<BVHMotionFrame> motionFrames;
    };

    struct BVHMotionFrame {
        glm::vec3 rootTranslation;
        std::vector<glm::vec3> jointRotation;
    };

    struct BVHRawData {
        BVHSkeleton skeleton;
        BVHMotionData motionData;
    };

    static std::optional<BVHRawData> ReadBVHAnimationFile(fs::path &&filePath) {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            LogSystem::Elf_Error("Cannot Open this file.");
            return std::nullopt;
        }

        BVHRawData returnData;

        std::string line;
        std::shared_ptr<BVHJoint> currJoint;
        std::stack<std::shared_ptr<BVHJoint>> jointStack;

        bool readMotion = false;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            // get a line of tokens
            std::vector<std::string> tokens;

            std::string inputToken;
            while (iss >> inputToken) {
                tokens.push_back(inputToken);
            }

            if (!readMotion) {
                // parse tokens each line
                if (tokens[0] == "HIERARCHY") {
                    LogSystem::Info("Begin Read HIERARCHY.");
                }

                if (tokens[0] == "MOTION") {
                    LogSystem::Info("Begin Read MOTION DATA.");
                }

                if (tokens[0] == "OFFSET") {
                    currJoint->translation = glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]),
                                                       std::stof(tokens[3]));
                }

                // assume channels should be valid integer
                if (tokens[0] == "CHANNELS") {
                    // default:
                    // channels == 6 XYZ position ZYX rotation
                    // channels == 3 ZYX rotation
                    currJoint->channels = std::stoi(tokens[1]);
                }

                // if begin a joint, add it on the top pf the stack
                if (tokens[0] == "{" && currJoint != nullptr) {
                    jointStack.push(currJoint);
                }

                // if finish a joint, push it from the stack
                if (tokens[0] == "}" && !jointStack.empty()) {
                    jointStack.pop();
                }

                // if read into a joint, create a new joint and add it into:
                // list -> our  final record
                // tree -> hierarchy
                // currentJoint -> current operated object
                // parent->childrens() -> actually still current operated object
                if (tokens[0] == "JOINT" || tokens[0] == "ROOT") {
                    // create a joint here
                    auto joint = std::make_shared<BVHJoint>();
                    // assign the name
                    joint->name = tokens[1];

                    // change the active joint for operation
                    currJoint = joint;

                    // if the stack is not empty, 
                    // assign the top element as the parent and add the joint as a child
                    if (!jointStack.empty()) {
                        joint->parent = jointStack.top();
                        joint->parent->childrens.emplace_back(
                                joint); // Here we just add the joint to the parent's childrens list.
                    }

                    // I used a dummy node as the parent of the real hips 
                    if (tokens[0] == "ROOT") {
                        returnData.skeleton.dummyRoot = std::make_shared<BVHJoint>();
                        returnData.skeleton.dummyRoot->name = FileSystem::GetCleanFilename(filePath.string());
                        returnData.skeleton.dummyRoot->translation = glm::vec3();
                        returnData.skeleton.dummyRoot->childrens.emplace_back(joint);

                        joint->parent = returnData.skeleton.dummyRoot;
                    }

                    // add the joint to the joints list
                    returnData.skeleton.joints.emplace_back(joint);
                }

                // Extra END Site Processing for end notes 
                if (tokens[0] == "End" && tokens[1] == "Site") {
                    // Move to the next line, which should be "{"
                    std::getline(file, line);
                    // This should be the "OFFSET" line for the End Site
                    std::getline(file, line);
                    std::istringstream ess(line);
                    std::vector<std::string> offsetTokens;
                    std::string offsetToken;
                    while (ess >> offsetToken) {
                        offsetTokens.push_back(offsetToken);
                    }

                    if (offsetTokens[0] == "OFFSET" && currJoint) {
                        currJoint->endSiteOffset = glm::vec3(std::stof(offsetTokens[1]), std::stof(offsetTokens[2]),
                                                             std::stof(offsetTokens[3]));
                    }

                    currJoint->isEnd = true;
                    // Move to the next line, which should be "}"
                    std::getline(file, line);
                }
            }

            if (tokens[0] == "Frames:") {
                returnData.motionData.frames = std::stof(tokens[1]);
            }

            if (tokens[0] == "Frame" && tokens[1] == "Time:") {
                returnData.motionData.frameTime = std::stof(tokens[2]);
                readMotion = true;
                continue;
            }

            if (readMotion && (tokens.size() == (returnData.skeleton.joints.size() * 3 + 3))) {

                BVHMotionFrame frame;
                frame.rootTranslation = glm::vec3(std::stof(tokens[0]),
                                                  std::stof(tokens[1]),
                                                  std::stof(tokens[2]));

                for (size_t idx = 3; idx < tokens.size(); idx += 3) {
                    frame.jointRotation.emplace_back(
                            std::stof(tokens[idx]),
                            std::stof(tokens[idx + 1]),
                            std::stof(tokens[idx + 2])
                    );
                }

                returnData.motionData.motionFrames.emplace_back(std::move(frame));
            }
        }

        file.close();

        return returnData;
    }

    struct BonePositions {
        glm::vec3 current;
        std::vector<BonePositions> childrens;
    };

    static auto PrintJointsGlobalPosition = [](BVHRawData &bvhRawData) {
        for (auto &joint: bvhRawData.skeleton.joints) {
            LogSystem::Debug("{}, {}", joint->name, glm::to_string(joint->globalLocation))
        }
    };

    // recurisve the assign the world position for each joint
    static void ApplyJointOffsets(std::shared_ptr<BVHJoint> &parentNode, BonePositions &worldPositionNode) {
        for (auto &sk: parentNode->childrens) {

            glm::vec3 parentLocation = parentNode->globalLocation;
            sk->globalLocation = sk->translation + parentLocation;

            // Create a BonePositions for the current joint and set its current position
            BonePositions jointNode;
            jointNode.current = sk->globalLocation;

            // If the joint has an EndSite, add its position as a child of the joint
            if (sk->isEnd) {
                auto endPosition = sk->endSiteOffset.value() + sk->globalLocation;
                BonePositions endNode;
                endNode.current = endPosition;
                jointNode.childrens.emplace_back(std::move(endNode));
            }

            // Now, recursively compute the positions of the children joints
            ApplyJointOffsets(sk, jointNode);

            // Finally, add the joint (with its children and possibly its EndSite) to the parent
            worldPositionNode.childrens.emplace_back(std::move(jointNode));
        }
    }

    // recurisve the assign the world position for each joint
    static void ApplyJointOffsets2(std::shared_ptr<BVHJoint> &parentNode, BonePositions &worldPositionNode) {
        for (auto &sk: parentNode->childrens) {

            // Apply the parent's global rotation to the joint's local rotation
            sk->globalRotationMatrix = parentNode->globalRotationMatrix * sk->rotationMatrix;

            // Apply the global rotation matrix to the joint's translation to get the global location
            glm::vec4 globalOffset = sk->globalRotationMatrix * glm::vec4(sk->translation, 0.0f);
            sk->globalLocation = glm::vec3(globalOffset) + parentNode->globalLocation;

            // Create a BonePositions for the current joint and set its current position
            BonePositions jointNode;
            jointNode.current = sk->globalLocation;

            // If the joint has an EndSite, add its position as a child of the joint
            if (sk->isEnd) {
                glm::vec4 endGlobalOffset = sk->globalRotationMatrix * glm::vec4(sk->endSiteOffset.value(), 0.0f);
                auto endPosition = glm::vec3(endGlobalOffset) + sk->globalLocation;
                BonePositions endNode;
                endNode.current = endPosition;
                jointNode.childrens.emplace_back(std::move(endNode));
            }

            // Now, recursively compute the positions of the children joints
            ApplyJointOffsets2(sk, jointNode);

            // Finally, add the joint (with its children and possibly its EndSite) to the parent
            worldPositionNode.childrens.emplace_back(std::move(jointNode));
        }
    }

    static void GenerateJointLinkages(float scalar, std::vector<Cone> &linkages, const BonePositions &bone) {
        // Print the current value
        LogSystem::Debug("Joint World Position {}", glm::to_string(bone.current))
        auto &start = bone.current;
        // Recursive call for all children
        for (const auto &child: bone.childrens) {
            auto &end = child.current;
            linkages.emplace_back(end * scalar, start * scalar);
            GenerateJointLinkages(scalar, linkages, child);
        }
    }


    static glm::mat4 BVHEulerToRotationMatrix(glm::vec3 &euler) {

        // identity matrix
        glm::mat4 rotation = glm::mat4(1.0f);

        // z-axis
        rotation = glm::rotate(rotation, glm::radians(euler.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // x-axis
        rotation = glm::rotate(rotation, glm::radians(euler.x), glm::vec3(1.0f, 0.0f, 0.0f));

        // y-axis
        rotation = glm::rotate(rotation, glm::radians(euler.y), glm::vec3(0.0f, 1.0f, 0.0f));


//        glm::vec3 a = glm::degrees(glm::eulerAngles(glm::quat(rotation)));

        return rotation;
    }


}

