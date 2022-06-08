#include "Light.h"
#include "ShaderProgram.h"

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {
	Light::Light(std::shared_ptr<ShaderProgram> pShaderProgram, Color lightColor, glm::vec3 lightPosition) :
		m_pShaderProgram(std::move(pShaderProgram)), m_lightColor(lightColor), m_lightPosition(lightPosition)
	{
		const GLfloat vertexCoords[] = {
				-1.0, -1.0, -1.0,	1.0, -1.0, -1.0,	1.0,  1.0, -1.0,	-1.0,  1.0, -1.0,
				-1.0, -1.0,  1.0,	1.0, -1.0,  1.0,	1.0,  1.0,  1.0,	-1.0,  1.0,  1.0
		};

		const GLuint indices[] = {
			0, 1, 2,	2, 3, 0,
			0, 1, 5,	5, 4, 0,
			0, 4, 7,	7, 4, 0,
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

	void Light::render() const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, m_lightPosition);
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		m_pShaderProgram->setMat4("modelMat", model);
		m_pShaderProgram->setVec3("lightColor", m_lightColor.ambient);

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}
}