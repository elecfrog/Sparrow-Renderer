/*
 * description: Implicit Cone 
 * author@elecfrog
 */
#pragma once


#include "Prototype.hpp"

namespace Sparrow
{
    struct Cone : Prototype
    {
        Vector3f tip{};
        Vector3f baseCenter{};

        explicit Cone(Vector3f tip, Vector3f baseCenter);

        // assume the height of the cone is 2, in y-axis
        Cone();

        void Render(Shader& shader, CameraComponent& camera, glm::mat4& matModel, const Light& light) override;

    private:
        // build up vertices
        void buildMeshFilter(uint32_t N, float r);
    };
}
