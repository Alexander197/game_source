#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {

	struct Color
	{
		Color(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		{
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
		}
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	class ShaderProgram;

	class Light
	{
	public:
		Light(std::shared_ptr<ShaderProgram> pShaderProgram, Color lightColor, glm::vec3 lightPosition);
		void render() const;

		void setColor(Color color) { m_lightColor = color; }
		void setPosition(glm::vec3 lightPosition) { m_lightPosition = lightPosition; }

		Color getColor() { return m_lightColor; }
		glm::vec3 getPosition() { return m_lightPosition; }

		Light(const Light&) = delete;
		Light& operator = (const Light&) = delete;

	private:
		glm::vec3 m_lightPosition;

		Color m_lightColor;

		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}