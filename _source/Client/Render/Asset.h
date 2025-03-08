#pragma once

#include "FileSystem/FileDefinitions.h"

// ----------- CEREAL Headers included -------------------
#include <cereal/archives/json.hpp>

namespace Sparrow
{
    enum class AssetType
    {
        MODEL,
        MESH,
        MATERIAL,
        TEXTURE2D,
        TEXTURECUBE,
        SHADER,
        ANIMATIONCLIP,
        SKELETON,
    };

    class Asset
    {
    public:
        uint32_t m_ID{0};
        AssetType m_Type{};
        FileMetaInfo m_FileMetaInfo{};

        // virtual void LoadAsset(const std::string& filePath) = 0;
        // virtual void SaveAsset(const std::string& filePath) = 0;
    };
}

#include "AssetDataSupport.inl"
