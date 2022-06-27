#include "Mesh.h"

#include "Renderer.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "../Resources/ResourceManager.h"

namespace RenderEngine {

	Mesh::Mesh(std::vector<std::shared_ptr<Texture2D>> dTextures,
			   std::vector<std::shared_ptr<Texture2D>> sTextures,
			   std::vector<GLfloat> vertexCoords,
			   std::vector<GLfloat> texCoords,
			   std::vector<GLfloat> normals,
			   std::vector<GLfloat> tangents,
			   std::vector<GLuint> indices) : m_DiffTextures(std::move(dTextures)), m_SpecTextures(std::move(sTextures))
	{
		m_vertexCoordsBuffer.init(vertexCoords.data(), sizeof(GLfloat) * vertexCoords.size());
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_texCoordsBuffer.init(texCoords.data(), sizeof(GLfloat) * texCoords.size());
		VertexBufferLayout texCoordsLayout;
		texCoordsLayout.addElementLayoutFloat(2, GL_FALSE);
		m_vertexArray.addBuffer(m_texCoordsBuffer, texCoordsLayout);	

		m_normBuffer.init(normals.data(), sizeof(GLfloat) * normals.size());
		VertexBufferLayout normLayout;
		normLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_normBuffer, normLayout);

		m_tanBuffer.init(tangents.data(), sizeof(GLfloat) * tangents.size());
		VertexBufferLayout tanLayout;
		tanLayout.addElementLayoutFloat(3, GL_FALSE);
		m_vertexArray.addBuffer(m_tanBuffer, tanLayout);

		m_indexBuffer.init(indices.data(), sizeof(GLuint) * indices.size());

		m_vertexCoordsBuffer.unbind();
		m_texCoordsBuffer.unbind();
		m_normBuffer.unbind();
		m_tanBuffer.unbind();
		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	void Mesh::render(std::shared_ptr<ShaderProgram> pShader) const
	{
		size_t activeTextureNr = 0;

		for (size_t i = 0; i < m_DiffTextures.size(); i++, activeTextureNr++)
		{
			glActiveTexture(GL_TEXTURE0 + activeTextureNr);
			pShader->setInt(std::string("material.texture_diffuse_" + i), activeTextureNr);
			m_DiffTextures[i]->bind();
		}
		for (size_t i = 0; i < m_SpecTextures.size(); i++, activeTextureNr++)
		{
			glActiveTexture(GL_TEXTURE0 + activeTextureNr);
			pShader->setInt(std::string("material.texture_specular_" + std::to_string(i)), activeTextureNr);
			m_SpecTextures[i]->bind();
		}

		Renderer::draw(m_vertexArray, m_indexBuffer, *pShader);
	}
}