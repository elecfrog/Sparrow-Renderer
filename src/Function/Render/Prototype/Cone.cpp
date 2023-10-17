#include "Cone.hpp"
#include "Header.h"

Cone::Cone(glm::vec3 tip, glm::vec3 baseCenter)
        : tip(tip), baseCenter(baseCenter) {
    float length = (float)(tip - baseCenter).length();
    buildMeshFilter(10, length / 20.f);
    meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerTriangle, MeshType::TexturedMesh, meshFilter);
}

Cone::Cone() : tip{0.f, 1.0f, 0}, baseCenter{0.f, -1.0f, 0} {
    buildMeshFilter(30, 1);
    meshRenderer = std::make_shared<MeshRenderer>(RenderMode::PerTriangle, MeshType::TexturedMesh, meshFilter);
}

void Cone::Render(Shader &shader, Camera &camera, glm::mat4 &matModel, const Light &light) {

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

void Cone::buildMeshFilter(uint32_t N, float r) {

    std::vector<AttribVertex> vertices(2 * N + 2);
    std::vector<std::uint32_t> indices(6 * N);

    vertices[2 * N + 0].position = tip;       // tip of cone
    vertices[2 * N + 1].position = baseCenter; // center of base

    // main normal direction (axis of cone)
    glm::vec3 normal = glm::normalize(baseCenter - tip);
    glm::vec3 tangent = Maths::ComputeOrthogonalVector(normal);
    glm::vec3 biTangent = glm::normalize(glm::cross(normal, tangent));

    for (uint32_t i = 0; i < N; i++) {
        float angle = i * 2 * Maths::M_PI / N;

        float nx = cos(angle);
        float ny = sin(angle);

        // current tangent direction offset 
        glm::vec3 offset = nx * tangent + ny * biTangent;

        vertices[i].position = baseCenter + r * offset;
        vertices[N + i].position = tip;

        // Setting the normals might be a bit more complex due to cone structure, 
        // but for simplicity, I'm setting them the same as the offset
        vertices[i].normal = offset;
        vertices[N + i].normal = offset;

        int j = (i + 1) % N; // index of next face
        // outer faces
        indices[3 * i + 0] = N + i;  // tip
        indices[3 * i + 1] = j;      // next base vertex
        indices[3 * i + 2] = i;      // current base vertex

        // base face
        indices[3 * N + 3 * i + 0] = 2 * N + 1; // center
        indices[3 * N + 3 * i + 1] = i;
        indices[3 * N + 3 * i + 2] = j;
    }

    meshFilter = std::make_shared<StaticMesh>(std::move(StaticMesh{vertices, indices}));
}
