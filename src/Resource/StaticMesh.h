#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Vertex_new.h"
#include <glad/glad.h>

// TODO Change it to Static Mesh
struct StaticMesh {

    /*  Rendering Structure*/
    std::vector<TexturedVertex_new> vertices;
    std::vector<GLuint> indices;
};

