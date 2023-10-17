/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Rotation {
    glm::quat quaternion{};
    glm::vec3 euler{};

    explicit Rotation(const glm::vec3& v) : quaternion(glm::quat(v)), euler(v) {}
};

struct Transform {
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scaling;

    Transform()
            : position(glm::vec3(0.f)), rotation(glm::vec3(0.f)), scaling(glm::vec3(1.f)) {}
};