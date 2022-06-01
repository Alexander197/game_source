#pragma once

#include <memory>
#include <string>
#include <map>

#include "glad/glad.h"
#include <glm/vec3.hpp>


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"

namespace RenderEngine {

	class ShaderProgram;
	//class Texture2D;

	class Model3D {
	public:
		Model3D(std::shared_ptr<Texture2D> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<GLfloat> vertexCoords, 
			std::vector<GLfloat> texCoords, std::vector<GLfloat> normCoords, std::vector<GLuint> indices);

		Model3D(const Model3D&) = delete;
		Model3D& operator = (const Model3D&) = delete;

		void render(const glm::vec3 position, const glm::vec3 size, const float rotation) const;
	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		VertexBuffer m_normCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}
