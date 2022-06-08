#include "Model3D.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine {
	Model3D::Model3D(std::shared_ptr<Texture2D> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<GLfloat> vertexCoords, 
		std::vector<GLfloat> texCoords, std::vector<GLfloat> normCoords, std::vector<GLuint> indices) :
		m_pTexture(std::move(pTexture)),
		m_pShaderProgram(std::move(pShaderProgram))
	{
		m_vertexCoordsBuffer.init(vertexCoords.data(), sizeof(GLfloat) * vertexCoords.size());
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_texCoordsBuffer.init(texCoords.data(), sizeof(GLfloat) * texCoords.size());
		VertexBufferLayout texCoordsLayout;
		texCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_texCoordsBuffer, texCoordsLayout);

		m_normCoordsBuffer.init(normCoords.data(), sizeof(GLfloat) * normCoords.size());
		VertexBufferLayout normCoordsLayout;
		normCoordsLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_normCoordsBuffer, normCoordsLayout);

		m_indexBuffer.init(indices.data(), sizeof(GLuint) * indices.size());

		m_texCoordsBuffer.unbind();
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	void Model3D::render(const glm::vec3 position, const glm::vec3 size, const float rotation) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, size);

		glm::mat3 nModel = glm::mat3(glm::transpose(glm::inverse(model)));

		m_pShaderProgram->setMat3("nModelMat", nModel);
		m_pShaderProgram->setMat4("modelMat", model);

		m_pShaderProgram->setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		m_pShaderProgram->setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		m_pShaderProgram->setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		m_pShaderProgram->setFloat("material.shininess", 32.0f);
		
		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}
}