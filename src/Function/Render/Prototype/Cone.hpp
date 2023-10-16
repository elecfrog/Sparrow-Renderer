/*
 * description: Implicit Cone 
 * author@elecfrog
 */
#pragma once


#include "Prototype.hpp"

struct Cone : Prototype {
    glm::vec3 tip{};
    glm::vec3 baseCenter{};

    explicit Cone(glm::vec3 tip, glm::vec3 baseCenter);

    // assume the height of the cone is 2, in y-axis
    Cone();

    void Render(Shader& shader, Camera &camera, glm::mat4 &matModel, const Light &light) override;

private:
    // build up vertices
    void buildMeshFilter(uint32_t N, float r);
};