#pragma once
#include "RenderDefinitions.h"
#include "Core/Utils/Utility.hpp"

#include <array>
#include <filesystem>
#include "Resource/Asset.h"
#include "RenderDefinitions.h"
#include "Resource/Asset.h"

class TextureCube : public Asset
{
public:
	explicit TextureCube(std::array<std::filesystem::path, 6>& paths);

	unsigned int slotID = 0;

	~TextureCube();

	void Bind(uint32_t slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
		// also there are a term - "Bindless Texture"
	}

	void Unbind(uint32_t slot) const
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}

	int GetWidth() const;
	int GetHeight() const;

	[[nodiscard]] std::array<std::string, 6> GetFileName() const;

	// virtual void LoadAsset(const std::string& filePath) override;
	// virtual void SaveAsset(const std::string& filePath) override;

	inline uint32_t GetRenderId() const { return m_RendererId;  }

public:
	uint32_t m_RendererId = 0;
	TextureData m_ImgaeData;
	std::array<std::filesystem::path, 6> m_filePaths;
};