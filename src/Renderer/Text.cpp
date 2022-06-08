#include "Text.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"

namespace RenderEngine {
	Text::Text(std::shared_ptr<RenderEngine::Texture2D> texture, std::shared_ptr<RenderEngine::ShaderProgram> shader) :
		m_pTexture(std::move(texture)), m_pShaderProgram(std::move(shader))
	{
		const GLfloat vertexCoords[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};
		const GLfloat texCoords[] = {
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 0.0f
		};

		const GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		m_vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);


		m_texCoordsBuffer.init(texCoords, sizeof(texCoords), GL_DYNAMIC_DRAW);
		VertexBufferLayout texCoordsLayout;
		texCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_texCoordsBuffer, texCoordsLayout);

		m_indexBuffer.init(indices, sizeof(indices));

		m_texCoordsBuffer.unbind();
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}

	void Text::render(std::string text, glm::vec2 position, glm::vec2 size, float layer)
	{
		m_pShaderProgram->use();
		m_pShaderProgram->setVec3("discardColor", glm::vec3(1.0f, 1.0f, 1.0f));
		m_pShaderProgram->setFloat("layer", layer);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		size_t length = text.size();

		int currentOffsetX = 0;
		int currentOffsetY = 0;

		RenderEngine::Renderer::setDepthTest(false);
		for (size_t i = 0; i < length; i++)
		{
			char c = text[i];

			if (c != '\n')
			{
				std::string currentLetter(1, c);
				auto& subTexture = m_pTexture->getSubTexture(currentLetter);

				const GLfloat texCoords[] = {
					subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
					subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
					subTexture.rightTopUV.x, subTexture.rightTopUV.y,
					subTexture.rightTopUV.x, subTexture.leftBottomUV.y
				};
				m_texCoordsBuffer.update(texCoords, sizeof(texCoords));

				glm::vec2 pos = position + glm::vec2(static_cast<float>(currentOffsetX), -static_cast<float>(currentOffsetY)) * size;

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(pos, 0.0f));
				model = glm::scale(model, glm::vec3(size, 1.0f));

				m_pShaderProgram->setMat4("modelMat", model);
				Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
				currentOffsetX++;
			}
			else
			{
				currentOffsetY++;
				currentOffsetX = 0;
			}
		}
		RenderEngine::Renderer::setDepthTest(true);
	}
}