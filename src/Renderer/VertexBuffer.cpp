#include "VertexBuffer.h"

namespace RenderEngine {
	VertexBuffer::VertexBuffer():m_ID(0)
	{

	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	VertexBuffer& VertexBuffer::operator = (VertexBuffer&& vertexBuffer) noexcept
	{
		m_ID = vertexBuffer.m_ID;
		vertexBuffer.m_ID = 0;
		return *this;
	}
	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
	{
		m_ID = vertexBuffer.m_ID;
		vertexBuffer.m_ID = 0;
	}

	void VertexBuffer::init(const void* data, const unsigned int size, const GLenum usage)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	}
	void VertexBuffer::update(const void* data, const unsigned int size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	}
	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}