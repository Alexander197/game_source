#include "SkyBox.h"

#include "ShaderProgram.h"

namespace RenderEngine {

	SkyBox::SkyBox(std::shared_ptr<CubeMap> pCubeMap, std::shared_ptr<ShaderProgram> pShaderProgram) :
		m_pShaderProgram(std::move(pShaderProgram)),
		m_pCubeMap(std::move(pCubeMap))
	{
		const GLfloat vertexCoords[] = {
			-1.0f, -1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,	 1.0f,  1.0f, -1.0,
			 1.0f,  1.0f, -1.0f,	 1.0f, -1.0f, -1.0f,	-1.0f, -1.0f, -1.0,

			 1.0f, -1.0f, -1.0f,	 1.0f,  1.0f, -1.0f,	 1.0f,  1.0f,  1.0,
			 1.0f,  1.0f,  1.0f,	 1.0f, -1.0f,  1.0f,	 1.0f, -1.0f, -1.0,

			 1.0f, -1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,	-1.0f,  1.0f,  1.0,
			-1.0f,  1.0f,  1.0f,	-1.0f, -1.0f,  1.0f,	 1.0f, -1.0f,  1.0,

			-1.0f, -1.0f,  1.0f,	-1.0f,  1.0f,  1.0f,	-1.0f,  1.0f, -1.0,
			-1.0f,  1.0f, -1.0f,	-1.0f, -1.0f, -1.0f,	-1.0f, -1.0f,  1.0,

			-1.0f,  1.0f,  1.0f,	 1.0f,  1.0f,  1.0f,	 1.0f,  1.0f, -1.0,
			 1.0f,  1.0f, -1.0f,	-1.0f,  1.0f, -1.0f,	-1.0f,  1.0f,  1.0,

			-1.0f, -1.0f,  1.0f,	-1.0f, -1.0f, -1.0f,	 1.0f, -1.0f, -1.0,
			 1.0f, -1.0f, -1.0f,	 1.0f, -1.0f,  1.0f,	-1.0f, -1.0f,  1.0
		};

		m_vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_vertexCoordsBuffer.unbind();
		m_vertexArray.unbind();
	}
	void SkyBox::render() const
	{
		glDepthMask(GL_FALSE);
		m_pShaderProgram->use();
		m_pCubeMap->bind();
		m_vertexArray.bind();

		glDrawArrays(GL_TRIANGLES, 0 , 36);
		glDepthMask(GL_TRUE);
	}
}