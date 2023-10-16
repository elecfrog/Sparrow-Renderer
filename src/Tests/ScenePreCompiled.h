#pragma once

#include <array>
#include <glm/glm.hpp>

#include <unordered_map>

// Include OpenGL Loader Header
#include <glad/glad.h>

// Include ImGui for rendering GUI
#include <imgui/imgui.h>

// Include Scene Framework
#include "Scene.hpp"

// Include RHI
#include "Function/Render/RHI/Shader.h"
#include "Function/Render/Texture2D.h"
#include "Function/Render/RHI/FrameBuffer.h"
#include "Function/Controller/Camera.h"
#include "Function/Render/Material.h"
#include "Function/Render/Model.h"

// Include GLM for Math computing
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/WindowSystem/WindowSystem.h"
#include "Resource/ResourceManager.h"

// LIGHT;
#include "Function/Render/Light.h"
Light light;


#include "PrimitiveObjects/UVSphere.hpp"
#include "PrimitiveObjects/Plane.hpp"
#include "Function/Physics/Cloth.h"
#include "Function/Render/TextureCube.h"

#include "Core/Maths/Maths.h"
#include "Core/Maths/MathUtils.h"

inline glm::mat4 model_matrix = glm::mat4(1.0f);

static void ExportOBJ(fs::path path, std::shared_ptr<Cloth> &cloth) {
    std::ofstream outAsset(path);
    cereal::BinaryOutputArchive ar(outAsset);
    auto nodes = cloth->GetNodes();
    ar(cereal::make_nvp("clothObject", nodes));
}

static std::vector<TexturedVertex_new> ImportOBJ(fs::path path) {
    std::vector<TexturedVertex_new> ret;
    std::ifstream inputAsset(path);
    cereal::BinaryInputArchive ar(inputAsset);
    ar(cereal::make_nvp("clothObject", ret));
    return std::move(ret);
}

// Scene 01 : Triangle
inline const char *simple_vertexShaderSource =
        "#version 450 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

inline const char *simple_fragmentShaderSource =
        "#version 450 core\n"
        "out vec4 color;\n"
        "uniform vec3 editColorRGB;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(editColorRGB.x, editColorRGB.y, editColorRGB.z, 1.0);\n"
        "}\0";

inline float simple_triangleVertices[9]{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
};

inline uint32_t simple_triangleIndices[3] = {  // note that we start from 0!
        0, 1, 2,  // first Triangle
};

inline float simple_rectangleVertices[12] = {
        0.5f, 0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f, 0.0f   // top left 
};

inline uint32_t simple_rectangleIndices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

namespace fs = std::filesystem;

constexpr float cube_AttribVertices[] =
        {
                // POSITIONS            // NORMALS             // UVS
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

constexpr glm::vec3 cubePositions[10] =
        {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3(2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f, 3.0f, -7.5f),
                glm::vec3(1.3f, -2.0f, -2.5f),
                glm::vec3(1.5f, 2.0f, -2.5f),
                glm::vec3(1.5f, 0.2f, -1.5f),
                glm::vec3(-1.3f, 1.0f, -1.5f)
        };

constexpr float planeVertices[] =
        {
                // Positions           // Normals         // Texture Coords
                -2.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-left
                2.0f, 0.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Top-right
                -2.0f, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Bottom-left
                2.0f, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Bottom-right
        };

constexpr unsigned int planeIndices[] =
        {
                0, 2, 1, // Triangle 1 (CCW)
                1, 2, 3  // Triangle 2 (CCW)
        };


constexpr float quadVertices[] =
        { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f, 1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 1.0f, 1.0f
        };

float rectangleVertices[] =
        {
                //  Coords   // texCoords
                1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 1.0f,

                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f, 1.0f
        };

constexpr float skyboxVertices[] = {
        // positions          
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

// All the faces of the cubemap (make sure they are in this exact order)
std::array<std::filesystem::path, 6> texCube_Cloud =
        {
                "./assets/Skybox/Cloud/right.jpg",
                "./assets/Skybox/Cloud/left.jpg",
                "./assets/Skybox/Cloud/top.jpg",
                "./assets/Skybox/Cloud/bottom.jpg",
                "./assets/Skybox/Cloud/front.jpg",
                "./assets/Skybox/Cloud/back.jpg"};

// All the faces of the cubemap (make sure they are in this exact order)
std::array<std::filesystem::path, 6> texCube_Stormy =
        {
                "./assets/Skybox/Stormy/right.jpeg",
                "./assets/Skybox/Stormy/left.jpeg",
                "./assets/Skybox/Stormy/top.jpeg",
                "./assets/Skybox/Stormy/bottom.jpeg",
                "./assets/Skybox/Stormy/front.jpeg",
                "./assets/Skybox/Stormy/back.jpeg"
        };

// All the faces of the cubemap (make sure they are in this exact order)
std::array<std::filesystem::path, 6> texCube_Lake =
        {
                "./assets/skybox/lake/right.jpg",
                "./assets/skybox/lake/left.jpg",
                "./assets/skybox/lake/top.jpg",
                "./assets/skybox/lake/bottom.jpg",
                "./assets/skybox/lake/front.jpg",
                "./assets/skybox/lake/back.jpg"
        };

/*
 * Specfication of Model Naming:
 * Texture: <modelName>_texture_<textureType>_<textureSlotID>
 * MODEL: <modelName>_<ModelType>
 */
const std::filesystem::path banana_texture_diffuse_0 = "res/Models/banana.png";
const std::filesystem::path banana_OBJ = "res/Models/banana.obj";

const std::filesystem::path planks_texture_diffuse_0 = "res/textures/planks.png";

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
constexpr float geometry_points[] = {
        // positions        // colors
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};

const std::unordered_map<std::string, std::string> k_ModelList
        {
                {std::string("banana_OBJ"),             std::string("res/Models/banana.obj")},
                {std::string("cornellbox_spheres_OBJ"), std::string("res/Models/CornellBox-Sphere.obj")},
                {std::string("two_spheres_OBJ"),        std::string("res/Models/TwoSpheres.obj")},
                {std::string("mantis_OBJ"),             std::string("res/Models/mantis/mantis.obj")},
                {std::string("mantis_FBX"),             std::string("res/Models/mantis/model.fbx")},
                {std::string("mantis_GLTF"),            std::string("res/Models/mantis/scene.gltf")},
                {std::string("sfCube_GLTF"),            std::string("res/Models/sf_cube/scene.gltf")},
                {std::string("cpCube_GLTF"),            std::string("res/Models/companion_cube/scene.gltf")},
                {std::string("iceCube_GLTF"),           std::string("res/Models/ice_cube/scene.gltf")},
                {std::string("basicCube_GLTF"),         std::string("res/Models/basic_cube/basic_cube.gltf")},
                {std::string("scifiCube_GLTF"),         std::string("res/Models/sf/sf.gltf")},
                {std::string("taunt_FBX"),              std::string("res/Models/Taunt.fbx")},
                {std::string("hhd_FBX"),                std::string("res/Models/hhd.fbx")},
                {std::string("halo"),                   std::string(
                        "res/Models/spartan_armour_mkv_-_halo_reach/scene.gltf")},
                {std::string("xbot"),                   std::string("res/Models/X Bot.fbx")},
        };

const std::unordered_map<std::string, std::filesystem::path> texList
        {
                {std::string("cpCube_rougness"), std::filesystem::path(
                        "res/Models/companion_cube/textures/DefaultMaterial_metallicRoughness.png")},
                {std::string("banana_Diffuse0"), std::filesystem::path("res/Models/banana.png")},

        };