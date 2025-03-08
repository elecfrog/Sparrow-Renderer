#include "Texture2D.h"
#include "Core/Utils/Utility.hpp"
#include "Platform/FileSystem/FileSystem.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>

Texture2D::Texture2D(const std::string& path, bool flip, bool mipmap, OpenGLTextureWrapMode textureWrapMode)
{
	m_Type = AssetType::TEXTURE2D;
	m_FileMetaInfo = FileMetaInfo{ path, FileSystem::GetFileExtension(path)};

	stbi_set_flip_vertically_on_load(flip);
	m_ImgaeData.data = stbi_load(path.c_str(), &m_ImgaeData.width, &m_ImgaeData.height, &m_ImgaeData.channels, STBI_rgb_alpha);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// setup default texture settings
	// [ref](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html section 5.26.1 t0 5.26.4)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(textureWrapMode)));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(textureWrapMode)));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_ImgaeData.width, m_ImgaeData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ImgaeData.data));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (mipmap)
		GenerateMipmaps();
}

Texture2D::~Texture2D()
{
	if (m_ImgaeData.data)
		stbi_image_free(m_ImgaeData.data);
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture2D::Bind(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture2D::Unbind(uint32_t slot) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

TextureData Texture2D::GetImageData() const
{
	return m_ImgaeData;
}

void Texture2D::GenerateMipmaps()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

std::unique_ptr<Texture2D> Texture2D::LoadTextur2D(const std::string& filename)
{
	return std::make_unique<Texture2D>(filename);
}
