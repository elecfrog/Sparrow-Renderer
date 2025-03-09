#pragma once

#include "Render/Model.h"
#include "Render/Texture2D.h"
#include "Asset.h"
#include "FileSystem/FileSystem.h"

namespace Sparrow
{
    class ResourceManager
    {
    public:
        ResourceManager() = default;
        ~ResourceManager() = default;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        [[nodiscard]] std::shared_ptr<Asset> LoadResource(const std::string& filename) const
        {
            const FilePath filePath = filename.c_str();
            std::string extension = filePath.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(), tolower);
            if (extension == ".gltf")
                return Model::LoadModel(filename);
            if (extension == ".fbx")
                return Model::LoadModel(filename);
            if (extension == ".obj")
                return Model::LoadModel(filename);
            if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
                return Texture2D::LoadTexture2D(filename);

            throw std::runtime_error("Unsupported file extension: " + extension);
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<Asset>> m_Resources;
    };
}
