#pragma once

#include <Core/Core.h>
#include "RenderDefinitions.h"
#include "Render/Asset.h"

namespace Sparrow
{
	class Texture2D : public Asset
	{
	public:
		Texture2D() = default;
		explicit Texture2D(const std::string& path, Bool flip = false, Bool mipmap = true, OpenGLTextureWrapMode textureWrapMode = OpenGLTextureWrapMode::Repeat);
		~Texture2D();

		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other) = delete;
		Texture2D&& operator=(Texture2D&& other) = delete;

		void Bind(UInt slot) const;
		void Unbind(UInt slot) const;

		[[nodiscard]] TextureData GetImageData() const;
		void GenerateMipmaps();

		static std::unique_ptr<Texture2D> LoadTexture2D(const std::string& filename);

		[[nodiscard]] UInt GetTextureId() const { return m_RendererID; }

	private:
		UInt slotId{0};
		UInt m_RendererID{0};
		TextureData m_ImgaeData;
	};
}