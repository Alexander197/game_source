#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture,
				   std::string initialSubTexture,
				   std::shared_ptr<ShaderProgram> pSharedProgram) : 
		m_pTexture(std::move(pTexture)), 													
		m_pShaderProgram(std::move(pSharedProgram)),
		m_lastFrameId(0)
	{
		const GLfloat vertexCoords[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		auto &subTexture = m_pTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat texCoords[] = {
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y
		};

		const GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		m_vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);


		m_texCoordsBuffer.init(texCoords, sizeof(texCoords));
		VertexBufferLayout texCoordsLayout;
		texCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_texCoordsBuffer, texCoordsLayout);

		m_indexBuffer.init(indices, sizeof(indices));

		m_texCoordsBuffer.unbind();
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	Sprite::~Sprite()	{}

	void Sprite::render(const glm::vec2 position, const glm::vec2 size, const float rotation, const size_t frameId ) const
	{
		if (m_lastFrameId != frameId)
		{
			m_lastFrameId = frameId;
			const FrameDescription& currentFrameDescription = m_framesDescriptions[frameId];

			const GLfloat texCoords[] = {
				currentFrameDescription.leftBottomUV.x, currentFrameDescription.leftBottomUV.y,
				currentFrameDescription.leftBottomUV.x, currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x, currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x, currentFrameDescription.leftBottomUV.y
			};

			m_texCoordsBuffer.update(texCoords, sizeof(texCoords));
		}
		
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));

		m_pShaderProgram->setMatrix4("modelMat", model);
		
		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}

	void Sprite::insertFrames(std::vector<FrameDescription> framesDescriptions)
	{
		m_framesDescriptions = std::move(framesDescriptions);
	}

	uint64_t Sprite::getFrameDuration(const size_t frameId) const
	{
		return m_framesDescriptions[frameId].duration;
	}

	size_t Sprite::getFramesCount() const
	{
		return m_framesDescriptions.size();
	}
}