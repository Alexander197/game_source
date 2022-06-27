#pragma once

#include <memory>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture2D.h"

namespace RenderEngine {

	class ShaderProgram;

	class Mesh {
	public:
		Mesh(std::vector<std::shared_ptr<Texture2D>> dTextures,
			std::vector<std::shared_ptr<Texture2D>> sTextures,
			 std::vector<GLfloat> vertexCoords,
			 std::vector<GLfloat> texCoords, 
			 std::vector<GLfloat> normals, 
			 std::vector<GLfloat> tangents, 
			 std::vector<GLuint> indices);
		void render(std::shared_ptr<ShaderProgram> pShader) const;
	private:
		VertexArray m_vertexArray;

		std::vector<std::shared_ptr<Texture2D>> m_DiffTextures;
		std::vector<std::shared_ptr<Texture2D>> m_SpecTextures;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		VertexBuffer m_normBuffer;
		VertexBuffer m_tanBuffer;

		IndexBuffer m_indexBuffer;
	};
}
