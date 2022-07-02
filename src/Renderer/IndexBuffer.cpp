#include "IndexBuffer.h"

namespace RenderEngine {
	IndexBuffer::IndexBuffer() :m_ID(0), m_count(0)
	{

	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	IndexBuffer& IndexBuffer::operator = (IndexBuffer&& indexBuffer) noexcept
	{
		m_ID = indexBuffer.m_ID;
		m_count = indexBuffer.m_count;
		indexBuffer.m_ID = 0;
		indexBuffer.m_count = 0;
		return *this;
	}
	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
	{
		m_ID = indexBuffer.m_ID;
		m_count = indexBuffer.m_count;
		indexBuffer.m_ID = 0;
		indexBuffer.m_count = 0;
	}

	void IndexBuffer::init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		m_count = size / sizeof(GLuint);
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}