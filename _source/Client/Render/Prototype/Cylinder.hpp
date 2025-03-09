/*
 * description: Implicit Cylinder 
 * author@elecfrog
 */
#pragma once


#include "Prototype.hpp"

namespace Sparrow
{
    // https://community.khronos.org/t/using-vbos-to-draw-a-cylinder-with-selectable-faces/107232/9
    struct Cylinder : Prototype
    {
        Vector3f start{};
        Vector3f end{};

        explicit Cylinder(Vector3f start, Vector3f end);

        // assume the length of the cylinder is 2, in y-axis
        Cylinder();


        void buildMeshFilter(uint32_t N, float r);

        void Render(Shader& shader, CameraComponent& camera, glm::mat4& matModel, const Light& light) override;
    };
}
