#include "FrameBuffer.h"
#include "Core/Log/LogSystem.h"

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, AttachmentFormat type)
	: m_Width(width), m_Height(height), m_Type(type)
{
	// Gen FrameBuffer
	GLCall(glGenFramebuffers(1, &m_FrameBufferID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

	// Gen FrameBufferTexture
	glGenTextures(1, &m_FrameBufferTextureID);
	glBindTexture(GL_TEXTURE_2D, m_FrameBufferTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<uint32_t>(m_Type), m_Width, m_Height, 0, static_cast<uint32_t>(m_Type),
	             GL_UNSIGNED_BYTE, nullptr);

	// Setup Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding

	// Create render buffer object for depth and stencil attachments
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameBufferTextureID, 0);
	glGenRenderbuffers(1, &m_RenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

	const auto& attachStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (attachStatus != GL_FRAMEBUFFER_COMPLETE)
		SPW_ERROR("Framebuffer not complete!");

	// Unbind(); 
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, AttachmentFormat type, std::int32_t num_attachments,
                         bool depth)
	: m_Width(width), m_Height(height), usingDepth(depth), m_Type(type),
	  numAttachments(num_attachments)
{
	// Gen FrameBuffer
	GLCall(glGenFramebuffers(1, &m_FrameBufferID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

	// Gen FrameBufferTexture
	m_TextureIDs.resize(numAttachments);
	glGenTextures(numAttachments, m_TextureIDs.data());

	/*
	 * essence: textures resource in the CPU pass to the GPU
	 * */
	for (unsigned int i = 0; i < m_TextureIDs.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureIDs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, (uint32_t)m_Type, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TextureIDs[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			SPW_ERROR("Framebuffer not complete!");
		}
	}

	if (usingDepth)
	{
		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

		/*
		 * essence: MRT attachments
		 * */
		std::vector<unsigned int> attachments;

		for (unsigned int i = 0; i < m_TextureIDs.size(); i++)
		{
			attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers(numAttachments, attachments.data());
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			SPW_ERROR("Framebuffer not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void FrameBuffer::Resize(int width, int height)
{
	// Gen FrameBuffer
	GLCall(glGenFramebuffers(1, &m_FrameBufferID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

	// Gen FrameBufferTexture
	if (!m_TextureIDs.empty())
	{
		for (unsigned int& m_TextureID : m_TextureIDs)
		{
			glDeleteTextures(1, &m_TextureID);
		}
	}

	m_TextureIDs.resize(numAttachments);
	glGenTextures(numAttachments, m_TextureIDs.data());

	/*
	 * essence: textures resource in the CPU pass to the GPU
	 * */
	for (unsigned int i = 0; i < m_TextureIDs.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureIDs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, (uint32_t)m_Type, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TextureIDs[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LogSystem::Debug("Framebuffer not complete!")
	}

	if (usingDepth)
	{
		if (m_RenderBufferID)
			glDeleteRenderbuffers(1, &m_RenderBufferID);

		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

		/*
		 * essence: MRT attachments
		 * */
		std::vector<unsigned int> attachments;

		for (unsigned int i = 0; i < m_TextureIDs.size(); i++)
		{
			attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glDrawBuffers(numAttachments, attachments.data());
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LogSystem::Debug("Framebuffer not complete!")
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}


FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteBuffers(1, &m_FrameBufferID));
	GLCall(glDeleteTextures(1, &m_FrameBufferTextureID));
	GLCall(glDeleteBuffers(1, &m_RenderBufferID));
}

void FrameBuffer::Bind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));
}

void FrameBuffer::Unbind() const
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::BindRead() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBufferID);
}

void FrameBuffer::BindDraw() const
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
