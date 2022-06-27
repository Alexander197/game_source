#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {
	class ShaderProgram;

	class LightSource
	{
	public:
		LightSource(std::shared_ptr<ShaderProgram> pShaderProgram, glm::vec3 lightPosition);
		void render() const;

		void setPosition(glm::vec3 lightPosition) { m_lightPosition = lightPosition; }

		glm::vec3 getPosition() { return m_lightPosition; }

		LightSource(const LightSource&) = delete;
		LightSource& operator = (const LightSource&) = delete;

	private:
		glm::vec3 m_lightPosition;

		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}