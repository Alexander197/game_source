#pragma once

#include <memory>
#include <string>
#include <map>

#include "glad/glad.h"
#include <glm/vec3.hpp>


#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"

namespace RenderEngine
{
	class ShaderProgram;

	class PlainMesh
	{
	public:
		PlainMesh(std::shared_ptr<ShaderProgram> pShaderProgram, size_t width, size_t heigth, float step);

		PlainMesh(const PlainMesh&) = delete;
		PlainMesh& operator = (const PlainMesh&) = delete;

		void render(const glm::vec3 position, const glm::vec3 size, const glm::vec3 rotation) const;
		
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		
		size_t m_width;
		size_t m_heigth;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_heightsBuffer;

		IndexBuffer m_indexBuffer;
	};
}
