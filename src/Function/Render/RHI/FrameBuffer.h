#pragma once
#include "Core/Utils/Utility.hpp"
#include <vector>
enum class AttachmentFormat
{
	RGB		= GL_RGB,
	RGBA16F	= GL_RGBA16F,
	DEPTH	= GL_DEPTH_COMPONENT,
};

class FrameBuffer
{
public:
	FrameBuffer(uint32_t width, uint32_t height, AttachmentFormat type);
	explicit FrameBuffer(uint32_t width, uint32_t height, AttachmentFormat type, std::int32_t numbers, bool depth);
	~FrameBuffer();
	void Bind() const;
	void BindRead() const;
	void Unbind() const;
	void BindDraw() const;

	void SetWidth(uint32_t w) { m_Width = w; };
	void SetHeight(uint32_t h) { m_Height = h; };
	[[nodiscard]] uint32_t GetFrameBufferTextureID() const { return m_FrameBufferTextureID; }

    std::vector<uint32_t> GetTextureIds() const
    {
        return m_TextureIDs;
    }

    void Resize(int width, int height);
private:
	uint32_t m_FrameBufferID, m_RenderBufferID, m_FrameBufferTextureID;
    std::vector<uint32_t> m_TextureIDs;
	uint32_t m_Width, m_Height;
    bool usingDepth = true;
	AttachmentFormat m_Type;
	uint32_t  numAttachments;
};

using FBO = FrameBuffer;