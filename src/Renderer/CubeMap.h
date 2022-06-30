#pragma once

#include "glad/glad.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace RenderEngine
{
	class CubeMap
	{
	public:
		CubeMap(const GLuint width,
			const GLuint height,
			const std::vector<unsigned char*> data,
			const unsigned int channels = 4,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		CubeMap() = delete;
		CubeMap(const CubeMap&) = delete;
		CubeMap& operator = (const CubeMap&) = delete;
		CubeMap& operator = (CubeMap&& texture2d) noexcept;
		CubeMap(CubeMap&& texture2d) noexcept;

		~CubeMap();

		void bind() const;
		void unbind() const;

	private:
		GLuint m_ID;
		GLenum m_mode;

		unsigned int m_width;
		unsigned int m_height;
	};
}