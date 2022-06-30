#pragma once

#include <memory>
#include <string>
#include <map>

#include "glad/glad.h"
#include <glm/vec3.hpp>


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "CubeMap.h"

namespace RenderEngine{

	class ShaderProgram;

	class SkyBox
	{
	public:
		SkyBox(std::shared_ptr<CubeMap> pCubeMap, std::shared_ptr<ShaderProgram> pShaderProgram);

		SkyBox(const SkyBox&) = delete;
		SkyBox& operator = (const SkyBox&) = delete;

		void render() const;

	private:
		std::shared_ptr<CubeMap> m_pCubeMap;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}