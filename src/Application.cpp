RegisterScene<Scene_BVHViewer>("BVH Animation Viewer");
RegisterScene<Scene_PBRTest>("PBR Test");
RegisterScene<Scene_LoadModel>("Forward+");
RegisterScene<Scene_Terrain>("Terrain");
RegisterScene<Scene06_ShadowMapping>("Shadow Mapping");
RegisterScene<Scene_ClothSimulationSS1>("Cloth Simulation SS1");
RegisterScene<Scene_ClothSimulationSS2>("Cloth Simulation SS2");


static void ExportOBJ(fs::path path, std::shared_ptr<Cloth> &cloth) {
    std::ofstream outAsset(path);
    cereal::BinaryOutputArchive ar(outAsset);
    auto nodes = cloth->GetNodes();
    ar(cereal::make_nvp("clothObject", nodes));
}

static std::vector<AttribVertex> ImportOBJ(fs::path path) {
    std::vector<AttribVertex> ret;
    std::ifstream inputAsset(path);
    cereal::BinaryInputArchive ar(inputAsset);
    ar(cereal::make_nvp("clothObject", ret));
    return std::move(ret);
}

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