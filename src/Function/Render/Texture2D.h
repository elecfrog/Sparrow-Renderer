#pragma once

#include "RenderDefinitions.h"
#include "Resource/Asset.h"

class Texture2D : public Asset
{
public:
	Texture2D() = default;
	explicit Texture2D(const std::string& path, bool flip = false, bool mipmap = true, OpenGLTextureWrapMode textureWrapMode  = OpenGLTextureWrapMode::Repeat);
	~Texture2D();

	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;
	Texture2D(Texture2D&& other) = delete;
	Texture2D&& operator=(Texture2D&& other) = delete;

	void Bind(uint32_t slot)   const; 
	void Unbind(uint32_t slot) const;

	[[nodiscard]] TextureData GetImageData() const;
	void GenerateMipmaps();

	static std::unique_ptr<Texture2D> LoadTextur2D(const std::string& filename);

	// virtual void LoadAsset(const std::string& filePath) override;
	// virtual void SaveAsset(const std::string& filePath) override;

	uint32_t GetTextureId() const
	{
		return m_RendererID;
	}

private:
	uint32_t	m_SlotID	 = 0;
	uint32_t	m_RendererID = 0;
	TextureData m_ImgaeData;
};