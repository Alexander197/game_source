#include "CubeMap.h"

namespace RenderEngine
{
	CubeMap::CubeMap(const GLuint width,
		const GLuint height,
		std::vector<unsigned char*> data,
		const unsigned int channels,
		const GLenum filter,
		const GLenum wrapMode) : m_width(width), m_height(height)
	{
		switch (channels)
		{
		case 4:
			m_mode = GL_RGBA;
			break;
		case 3:
			m_mode = GL_RGB;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}
		size_t size = data.size();

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		for (size_t i = 0; i < size; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data[i]);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	CubeMap& CubeMap::operator = (CubeMap&& cubeMap) noexcept
	{
		glDeleteTextures(1, &m_ID);
		m_ID = cubeMap.m_ID;
		cubeMap.m_ID = 0;
		m_mode = cubeMap.m_mode;
		m_width = cubeMap.m_width;
		m_height = cubeMap.m_height;
		return *this;
	}
	CubeMap::CubeMap(CubeMap&& cubeMap) noexcept
	{
		m_ID = cubeMap.m_ID;
		cubeMap.m_ID = 0;
		m_mode = cubeMap.m_mode;
		m_width = cubeMap.m_width;
		m_height = cubeMap.m_height;
	}

	CubeMap::~CubeMap()
	{
		glDeleteTextures(1, &m_ID);
	}

	void CubeMap::bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

	void CubeMap::unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}