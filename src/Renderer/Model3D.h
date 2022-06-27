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

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RenderEngine {

	struct Light
	{
		Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
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

	class Model3D {
	public:
		//Model3D(std::shared_ptr<Texture2D> pTexture, std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<GLfloat> vertexCoords, 
		//	std::vector<GLfloat> texCoords, std::vector<GLfloat> normals, std::vector<GLfloat> tangents, std::vector<GLuint> indices);
		
		Model3D(std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<std::shared_ptr<Mesh>> pMeshes);

		Model3D(const Model3D&) = delete;
		Model3D& operator = (const Model3D&) = delete;

		void render(const glm::vec3 position, const glm::vec3 size, const glm::vec3 rotation) const;

		static std::vector<std::shared_ptr<Mesh>> loadMeshes(std::string path);
	private:
		static void processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>> &meshes, std::string path);
		static std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, std::string path);
		

		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		std::vector<std::shared_ptr<Mesh>> m_pMeshes;

		//std::shared_ptr<Texture2D> m_pTexture;

		Light m_ligth;

		//VertexArray m_vertexArray;

		//VertexBuffer m_vertexCoordsBuffer;
		//VertexBuffer m_texCoordsBuffer;
		//VertexBuffer m_normBuffer;
		//VertexBuffer m_tanBuffer;

		//IndexBuffer m_indexBuffer;
	};
}
