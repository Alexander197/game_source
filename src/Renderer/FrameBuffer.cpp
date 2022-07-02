#include "FrameBuffer.h"

#include <iostream>

namespace RenderEngine
{
	FrameBuffer::FrameBuffer() : m_ID(0), m_textureColorBuffer(0)
	{ }

	FrameBuffer::~FrameBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		glDeleteTextures(1, &m_textureColorBuffer);
	}

	FrameBuffer& FrameBuffer::operator = (FrameBuffer&& m_frameBuffer) noexcept
	{
		m_ID = m_frameBuffer.m_ID;
		m_frameBuffer.m_ID = 0;
		m_textureColorBuffer = m_frameBuffer.m_textureColorBuffer;
		m_frameBuffer.m_textureColorBuffer = 0;
		return *this;
	}
	FrameBuffer::FrameBuffer(FrameBuffer&& m_frameBuffer) noexcept
	{
		m_ID = m_frameBuffer.m_ID;
		m_frameBuffer.m_ID = 0;
		m_textureColorBuffer = m_frameBuffer.m_textureColorBuffer;
		m_frameBuffer.m_textureColorBuffer = 0;
	}

	void FrameBuffer::init(const glm::ivec2 windowSize)
	{
		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBuffer, 0);

		GLuint RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "ERROR::FRAMEBUFFER: Framebuffer is not complete" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::setWindowSize(const glm::ivec2 windowSize)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void FrameBuffer::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}
	void FrameBuffer::unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void FrameBuffer::bindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
	}
}