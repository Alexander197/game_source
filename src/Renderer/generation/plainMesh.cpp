#include "plainMesh.h"
#include "../Renderer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	PlainMesh::PlainMesh(std::shared_ptr<ShaderProgram> pShaderProgram, size_t width, size_t heigth, float step) :
		m_pShaderProgram(std::move(pShaderProgram)), m_width(width), m_heigth(heigth)
	{
		std::vector<float> vertexCoords;
		std::vector<float> heights;

		for (size_t i = 0; i < heigth; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				vertexCoords.push_back(static_cast<float>(j) * step);
				vertexCoords.push_back(static_cast<float>(i) * step);

				float h = static_cast<float>(rand() % 100) / 100.0f;
				heights.push_back(h);
			}
		}
		m_vertexCoordsBuffer.init(vertexCoords.data(), sizeof(GLfloat) * vertexCoords.size());
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_heightsBuffer.init(heights.data(), sizeof(GLfloat) * heights.size());
		VertexBufferLayout heightsLayout;
		heightsLayout.addElementLayoutFloat(1, GL_FALSE);
		m_vertexArray.addBuffer(m_heightsBuffer, heightsLayout);

		std::vector<GLuint> indices;

		for (size_t i = 0; i < heigth - 1; i++)
		{
			for (size_t j = 0; j < width - 1; j++)
			{
				indices.push_back(j + width * i);
				indices.push_back(j + width * (i + 1));
				indices.push_back(j + 1 + width * i);

				indices.push_back(j + 1 + width * i);
				indices.push_back(j + width * (i + 1));
				indices.push_back(j + width * (i + 1) + 1);
			}
		}

		m_indexBuffer.init(indices.data(), sizeof(GLuint) * indices.size());

		m_vertexArray.unbind();
		m_vertexCoordsBuffer.unbind();
		m_indexBuffer.unbind();
	}

	void PlainMesh::render(const glm::vec3 position, const glm::vec3 size, const glm::vec3 rotation) const
	{
 		/*
		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);*/
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, position);

		float pitch = glm::radians(rotation.x);
		float yaw = glm::radians(rotation.y);
		float roll = glm::radians(rotation.z);

		model = glm::rotate(model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		m_pShaderProgram->setMat4("modelMat", model);

		m_vertexArray.bind();

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	}
}