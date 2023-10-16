#include "Cylinder.hpp"
#include "Header.h"

Cylinder::Cylinder(glm::vec3 start, glm::vec3 end)
        : start(start), end(end) {
    // forward support for animation
    buildMeshFilter(30, 1.3f);
    meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerTriangle, MeshType::TexturedMesh, meshFilter);
}

Cylinder::Cylinder() : start{0.f, 1.0f, 0}, end{0.f, -1.0f, 0} {
    buildMeshFilter(30, 1.f);
    meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerTriangle, MeshType::TexturedMesh, meshFilter);
}

void Cylinder::buildMeshFilter(uint32_t N, float r) {

    std::vector<TexturedVertex_new> vertices(4 * N + 2);
    std::vector<std::uint32_t> indices(12 * N);


    vertices[4 * N + 0].position = start; // top centre
    vertices[4 * N + 1].position = end; // bottom centre

    glm::vec3 top_normal = glm::normalize(start - end);
    glm::vec3 bottom_normal = -top_normal;

    vertices[4 * N + 0].normal = top_normal; // top centre
    vertices[4 * N + 1].normal = bottom_normal; // bottom centre

    // main normal direction (axis of cylinder)
    glm::vec3 normal = glm::normalize(end - start);
    glm::vec3 tangent = Maths::ComputeOrthogonalVector(normal);
    glm::vec3 biTangent = glm::normalize(glm::cross(normal, tangent));


    for (std::uint32_t i = 0; i < N; i++) {
        float angle = i * 2 * Maths::M_PI / N;

        float nx = cos(angle);
        float ny = sin(angle);

        // current tangent direction offset 
        glm::vec3 offset = nx * tangent + ny * biTangent;

        vertices[2 * i + 0].position = vertices[2 * N + i].position = start + r * offset;
        vertices[2 * i + 1].position = vertices[3 * N + i].position = end + r * offset;

        vertices[2 * i + 0].normal = offset;
        vertices[2 * i + 1].normal = offset;
        vertices[2 * N + i].normal = top_normal;
        vertices[3 * N + i].normal = bottom_normal;


        int j = (i + 1) % N; // index of next face
        // outer faces
        // lower-right triangle
        indices[6 * i + 0] = 2 * j + 0; // lower-right
        indices[6 * i + 1] = 2 * j + 1; // upper-right
        indices[6 * i + 2] = 2 * i + 0; // lower-left
        // upper-left triangle
        indices[6 * i + 3] = 2 * j + 1; // upper-right
        indices[6 * i + 4] = 2 * i + 1; // upper-left
        indices[6 * i + 5] = 2 * i + 0; // lower-left
        // bottom face
        indices[6 * N + 3 * i + 1] = 2 * N + j;
        indices[6 * N + 3 * i + 0] = 2 * N + i;
        indices[6 * N + 3 * i + 2] = 4 * N + 0; // centre
        // top face
        indices[9 * N + 3 * i + 0] = 3 * N + i;
        indices[9 * N + 3 * i + 1] = 3 * N + j;
        indices[9 * N + 3 * i + 2] = 4 * N + 1; // centre
    }

    meshFilter = std::make_shared<StaticMesh>(std::move(StaticMesh{vertices, indices}));
}

void Cylinder::Render(Shader &shader, Camera &camera, glm::mat4 &matModel, const Light &light) {

    shader.Bind()
                    // setup MVP models
            .SetUniformMat4f("M", matModel)
            .SetUniformMat4f("V", camera.viewMatrix)
            .SetUniformMat4f("P", camera.projMatrix)
                    // setup light attributes
            .SetUniform3f("light.diffuseColor", light.color)
            .SetUniform3f("light.ambientColor", light.ambient_color)
            .SetUniform3f("light.position", light.position)
            .SetUniform3f("viewPos", camera.cameraPos)
                    // setup material attributes
            .SetUniform1i("tex_Diffuse", 1)
            .SetUniform4f("material.baseColor", glm::vec4(243.0f / 255.0f, 200.0f / 255.0f, 203.0f / 255.0f, 1.0f));

    // DrawCall
    meshRenderer->Render();

    shader.Unbind();
}
