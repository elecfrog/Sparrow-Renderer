/*
 * description: Implicit Cylinder 
 * author@elecfrog
 */
#pragma once


#include "Prototype.hpp"

// https://community.khronos.org/t/using-vbos-to-draw-a-cylinder-with-selectable-faces/107232/9
struct Cylinder : Prototype {

    glm::vec3 start{};
    glm::vec3 end{};

    explicit Cylinder(glm::vec3 start, glm::vec3 end);

    // assume the length of the cylinder is 2, in y-axis
    Cylinder();


    void buildMeshFilter(uint32_t N, float r);

    void Render(Shader &shader, Camera &camera, glm::mat4 &matModel, const Light &light) override;
};