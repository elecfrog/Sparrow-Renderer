#include <array>
#include <filesystem>

#include <stb_image.h>

#include "Core/Utils/Utility.hpp"
#include "TextureCube.h"
#include "Platform/FileSystem/FileSystem.h"

TextureCube::TextureCube(std::array<std::filesystem::path, 6>& paths)
{
	// Creates the cubemap texture object
	glGenTextures(1, &m_RendererId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(paths[i].string().c_str(), &m_ImgaeData.width, &m_ImgaeData.height, &m_ImgaeData.channels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				m_ImgaeData.width,
				m_ImgaeData.height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
			// std::cout << "load texture scuess: " << paths[i] << std::endl;
		}
		else
		{
			// std::cout << "Failed to load texture: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}
}

TextureCube::~TextureCube()
{
	GLCall(glDeleteTextures(1, &m_RendererId));
}

int TextureCube::GetWidth() const
{
	return m_ImgaeData.width;
}

int TextureCube::GetHeight() const
{
	return m_ImgaeData.height;
}

std::array<std::string, 6> TextureCube::GetFileName() const
{
	std::array<std::string, 6> tmp;
	for(unsigned int i =0; i<6; ++i)
	{
		tmp[i] = m_filePaths[i].filename().string();
	}
	return tmp;
}
