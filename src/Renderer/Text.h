#pragma once
#include <string>
#include <memory>
#include "Texture2D.h"
#include "ShaderProgram.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {
	class Text
	{
	public:
		Text(std::shared_ptr<RenderEngine::Texture2D> texture, std::shared_ptr<RenderEngine::ShaderProgram> shader);

		void render(std::string text, glm::vec2 position, glm::vec2 size, float layer);

	private:
		std::shared_ptr<RenderEngine::Texture2D> m_pTexture;
		std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}
