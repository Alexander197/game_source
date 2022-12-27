#include "PostProcess.h"

#include "Renderer.h"

namespace RenderEngine
{
	PostProcess::PostProcess(std::shared_ptr<ShaderProgram> pSharedProgram, glm::ivec2 windowSize) :
		m_pShaderProgram(std::move(pSharedProgram))
	{
		m_frameBuffer.init(windowSize);

		const GLfloat coords[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,
			 1.0f,  1.0f,
			-1.0f,  1.0f,
			-1.0f, -1.0f
		};

		const GLfloat tcoords[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f
		};

		m_vertexCoordsBuffer.init(coords, sizeof(coords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);


		m_texCoordsBuffer.init(tcoords, sizeof(tcoords));
		VertexBufferLayout texCoordsLayout;
		texCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_texCoordsBuffer, texCoordsLayout);

		m_vertexCoordsBuffer.unbind();
		m_texCoordsBuffer.unbind();
		m_vertexArray.unbind();
	}

	void PostProcess::setWindowSize(const glm::ivec2 windowSize)
	{
		m_frameBuffer.setWindowSize(windowSize);
	}

	void PostProcess::capture() const
	{
		m_frameBuffer.bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	void PostProcess::render() const
	{
		m_frameBuffer.unbind();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_pShaderProgram->use();
		m_vertexArray.bind();
		glDisable(GL_DEPTH_TEST);
		
		m_pShaderProgram->setInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		m_frameBuffer.bindTexture();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
	}
}