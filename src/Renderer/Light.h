#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {

	class ShaderProgram;

	class Light
	{
	public:
		Light(std::shared_ptr<ShaderProgram> pShaderProgram, glm::vec3 lightColor, glm::vec3 lightPosition);

		void render() const;

		void setColor(glm::vec3 lightColor) { m_lightColor = lightColor; }
		void setPosition(glm::vec3 lightPosition) { m_lightPosition = lightPosition; }

		glm::vec3 getColor() { return m_lightColor; }
		glm::vec3 getPosition() { return m_lightPosition; }

		Light(const Light&) = delete;
		Light& operator = (const Light&) = delete;

	private:
		glm::vec3 m_lightColor;
		glm::vec3 m_lightPosition;

		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}