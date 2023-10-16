#pragma once

#include "Resource/Asset.h"
#include "Function/Render/Mesh.h"
#include "Function/Render/Skin.h"

#define USE_ASSIMP
class Model : public Asset
{
public:
    Model() = default;
    ~Model() = default;

	static std::unique_ptr<Model> LoadModel(const std::string& filename);
    
    [[nodiscard]] const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
    void AddMesh(std::shared_ptr<Mesh> mesh) { m_Meshes.emplace_back(mesh); }

    // void LoadAsset(const std::string& filePath) override;
    // void SaveAsset(const std::string& filePath) override;

public:
    std::vector<std::shared_ptr<Mesh>>              m_Meshes;
    std::shared_ptr<BoneInfo>                       m_BoneInfo;

private:
#ifdef USE_ASSIMP
    static std::unique_ptr<Model> LoadByAssimp(const std::string& filename);
#endif

	void LoadOBJ(const std::string& filename);
    void LoadFBX(const std::string& filename);
    void LoadGLTF(const std::string& filename);
};

