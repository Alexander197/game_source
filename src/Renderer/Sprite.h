#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(std::shared_ptr<Texture2D> pTexture,
			   std::string initialSubTexture,
			   std::shared_ptr<ShaderProgram> pSharedProgram);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator = (const Sprite&) = delete;

		virtual void render(const glm::vec2 position, const glm::vec2 size, const float rotation) const;

	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}
