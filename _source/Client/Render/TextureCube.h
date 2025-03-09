#pragma once

#include "Base/BaseDefinition.h"
#include "RenderDefinitions.h"
#include "Render/Asset.h"

#include <array>
#include <filesystem>

namespace Sparrow
{
    class TextureCube : public Asset 
    {
    public:
        explicit TextureCube(std::array<std::filesystem::path, 6>& paths);
        ~TextureCube();

        void Bind(UInt slot) const 
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + slot));
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
            // also there are a term - "Bindless Texture"
        }

        void Unbind(UInt slot) const 
        {
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        }

        [[nodiscard]] Int GetWidth() const;
        [[nodiscard]] Int GetHeight() const;
        [[nodiscard]] std::array<std::string, 6> GetFileName() const;
        [[nodiscard]] UInt GetRenderId() const { return m_RendererId; }

    public:
        UInt m_RendererId{0};
        UInt slotId{0};
        TextureData imageData;
        std::array<std::filesystem::path, 6> filePaths;
    };
}