#include "LightSource.h"
#include "ShaderProgram.h"

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {
	LightSource::LightSource(std::shared_ptr<ShaderProgram> pShaderProgram, glm::vec3 lightPosition) :
		m_pShaderProgram(std::move(pShaderProgram)), m_lightPosition(lightPosition)
	{
		const GLfloat vertexCoords[] = {
				-1.0, -1.0, -1.0,	1.0, -1.0, -1.0,	1.0,  1.0, -1.0,	-1.0,  1.0, -1.0,
				-1.0, -1.0,  1.0,	1.0, -1.0,  1.0,	1.0,  1.0,  1.0,	-1.0,  1.0,  1.0
		};

		const GLuint indices[] = {
			0, 3, 2,	2, 1, 0,
			0, 1, 5,	5, 4, 0,
			0, 4, 7,	7, 3, 0,
			1, 2, 6,	6, 5, 1,
			2, 3, 7,	7, 6, 2,
			4, 5, 6,	6, 7, 4
		};
		m_vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_indexBuffer.init(indices, sizeof(indices));

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}

	void LightSource::render() const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, m_lightPosition);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		m_pShaderProgram->setMat4("modelMat", model);	

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}
}