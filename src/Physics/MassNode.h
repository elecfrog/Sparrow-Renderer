#pragma once

#include "Resource/Vertex_new.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

struct MassNode {

    explicit MassNode(glm::vec3 position)
            : mass(1.0), velocity(0.0), acceleration(0), force(0), isFixed(false) {

        vertex.position = position;
        oldPos = position;
    }

    // physics
    float mass = 1.0f;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);


    // graphics
    AttribVertex vertex{};
    glm::vec3 oldPos{};

    bool isFixed = false;
};




