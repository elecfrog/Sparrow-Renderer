#pragma once

#include <vector>
#include <glad/glad.h>
#include "Render/Vertex.h"

namespace Sparrow
{
    struct StaticMesh
    {
        /*  Rendering Structure*/
        std::vector<AttribVertex> vertices;
        std::vector<GLuint> indices;
    };
}
