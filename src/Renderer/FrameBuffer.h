#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace RenderEngine 
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer& operator = (const FrameBuffer&) = delete;
		FrameBuffer& operator = (FrameBuffer&& m_frameBuffer) noexcept;
		FrameBuffer(FrameBuffer&& m_frameBuffer) noexcept;

		void init(const glm::ivec2 windowSize);

		void setWindowSize(const glm::ivec2 windowSize);

		void bind() const;
		void unbind() const;
		void bindTexture() const;

	private:
		GLuint m_ID;
		GLuint m_textureColorBuffer;
	};
}