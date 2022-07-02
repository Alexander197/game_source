#pragma once

#include <glad/glad.h>

namespace RenderEngine {

	class IndexBuffer {
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator = (const IndexBuffer&) = delete;
		IndexBuffer& operator = (IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		void init(const void* data, const unsigned int size);
		void bind() const;
		void unbind() const;
		unsigned int getCount() const { return m_count; }

	private:
		GLuint m_ID;
		unsigned int m_count;
	};

}