#pragma once

#include <vector>
#include <glad/glad.h>
#include "Vertex_new.h"

struct StaticMesh {

    /*  Rendering Structure*/
    std::vector<AttribVertex> vertices;
    std::vector<GLuint> indices;
};

