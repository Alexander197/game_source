#pragma once
#include <memory>

#include "FrameBuffer.h"
#include <glm/vec2.hpp>
#include "ShaderProgram.h"

#include "VertexArray.h"
#include "VertexBuffer.h"


namespace RenderEngine
{
	class PostProcess
	{
	public:
		PostProcess(std::shared_ptr<ShaderProgram> pSharedProgram, glm::ivec2 windowSize);

		PostProcess(const PostProcess&) = delete;
		PostProcess& operator = (const PostProcess&) = delete;

		void setWindowSize(const glm::ivec2 windowSize);

		void capture() const;
		void render() const;

	private:
		FrameBuffer m_frameBuffer;
		
		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
	};
}