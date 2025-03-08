#pragma once
#include <memory>
#include <string>
#include "Platform/FileSystem/FileDefinitions.h"

// ----------- CEREAL Headers included -------------------
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

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

	uint32_t        m_ID{0};
    AssetType       m_Type{};
    FileMetaInfo    m_FileMetaInfo{};

    // virtual void LoadAsset(const std::string& filePath) = 0;
    // virtual void SaveAsset(const std::string& filePath) = 0;
};

#include "AssetDataSupport.inl"