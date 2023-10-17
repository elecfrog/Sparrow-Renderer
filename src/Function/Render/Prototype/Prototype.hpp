/*
 * description:
 * author@elecfrog
 */
#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

struct StaticMesh;
struct MeshRenderer;
struct AttribVertex;
struct Shader;
struct Camera;
struct Light;

struct Prototype {
    Prototype() = default;

    virtual ~Prototype() = default;

    virtual void Render(Shader& shader, Camera &camera, glm::mat4 &matModel, const Light &light) = 0;

    std::shared_ptr<StaticMesh> meshFilter;
    std::shared_ptr<MeshRenderer> meshRenderer;
    std::vector<AttribVertex> vertices;
};
