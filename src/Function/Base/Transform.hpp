/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Rotation {
    glm::quat quaternion /*= glm::quat()*/;
    glm::vec3 euler/* = glm::vec3()*/;

    explicit Rotation(glm::vec3 v)
            : euler(v), quaternion(glm::quat(v)) {}
};

struct Transform {
    glm::vec3 position;
    Rotation rotation;
    glm::vec3 scaling;

    Transform()
            : position(glm::vec3(0.f)), rotation(glm::vec3(0.f)), scaling(glm::vec3(1.f)) {}
};