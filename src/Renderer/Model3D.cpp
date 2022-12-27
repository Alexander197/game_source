#include "Model3D.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "Renderer.h"

#include "../Resources/ResourceManager.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace RenderEngine {
	Model3D::Model3D(std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<std::shared_ptr<Mesh>> pMeshes) :
		m_pShaderProgram(std::move(pShaderProgram)),
		m_pMeshes(pMeshes),
		m_stencilColor(glm::vec3(1.0f, 1.0f, 1.0f)), m_stencilExpantion(1.1f)
	{

	}
	void Model3D::render(const glm::vec3 position, const glm::vec3 size, const glm::vec3 rotation, const bool withStencil) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);
		
		model = glm::translate(model, position);

		float pitch = glm::radians(rotation.x);
		float yaw = glm::radians(rotation.y);
		float roll = glm::radians(rotation.z);

		model = glm::rotate(model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		glm::mat3 nModel = glm::mat3(glm::transpose(glm::inverse(model)));

		m_pShaderProgram->setMat3("nModelMat", nModel);
		m_pShaderProgram->setMat4("modelMat", model);

		//m_pShaderProgram->setVec3("dirLight[0].ambient", m_ligth.ambient);
		//m_pShaderProgram->setVec3("dirLight[0].diffuse", m_ligth.diffuse);
		//m_pShaderProgram->setVec3("dirLight[0].specular", m_ligth.specular);

		//m_pShaderProgram->setVec3("dirLight[1].ambient", m_ligth.ambient);
		//m_pShaderProgram->setVec3("dirLight[1].diffuse", m_ligth.diffuse);
		//m_pShaderProgram->setVec3("dirLight[1].specular", m_ligth.specular);

		//m_pShaderProgram->setVec3("dirLight[0].ambient", m_ligth.ambient);
		//m_pShaderProgram->setVec3("dirLight[0].diffuse", m_ligth.diffuse);
		//m_pShaderProgram->setVec3("dirLight[0].specular", m_ligth.specular);

		//m_pShaderProgram->setFloat("pointLight[0].constant", 1.0);
		//m_pShaderProgram->setFloat("pointLight[0].linear", 0.014);
		//m_pShaderProgram->setFloat("pointLight[0].quadratic", 0.0007);

		//m_pShaderProgram->setVec3("spotLight[0].ambient", m_ligth.ambient * 1.0f);
		//m_pShaderProgram->setVec3("spotLight[0].diffuse", m_ligth.diffuse * 1.0f);
		//m_pShaderProgram->setVec3("spotLight[0].specular", m_ligth.specular * 1.0f);

		//m_pShaderProgram->setFloat("spotLight[0].constant", 1.0);
		//m_pShaderProgram->setFloat("spotLight[0].linear", 0.0014);
		//m_pShaderProgram->setFloat("spotLight[0].quadratic", 0.00007);

		m_pShaderProgram->setFloat("material.shininess", 32.0f);
		
		for (size_t i = 0; i < m_pMeshes.size(); i++)
		{
			m_pMeshes[i]->render(m_pShaderProgram);
		}
	}
	void Model3D::depthRender(const glm::vec3 position, const glm::vec3 size, const glm::vec3 rotation, const std::shared_ptr<ShaderProgram> pDepthShader) const
	{
		pDepthShader->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, position);

		float pitch = glm::radians(rotation.x);
		float yaw = glm::radians(rotation.y);
		float roll = glm::radians(rotation.z);

		model = glm::rotate(model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, roll, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size);

		pDepthShader->setMat4("modelMat", model);

		for (size_t i = 0; i < m_pMeshes.size(); i++)
		{
			m_pMeshes[i]->render(pDepthShader);
		}
	}

	std::vector<std::shared_ptr<Mesh>> Model3D::loadMeshes(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

		std::vector<std::shared_ptr<Mesh>> meshes;

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "ERROR ASSIMP: " << importer.GetErrorString() << std::endl;
			return meshes;
		}

		processNode(scene->mRootNode, scene, meshes, path);
		return meshes;
	}

	void Model3D::processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>> &meshes, std::string path)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(std::move(processMesh(mesh, scene, path)));
		}

		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, meshes, path);
		}
	}

	std::vector<std::shared_ptr<Texture2D>> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string path);
	std::shared_ptr<Mesh> Model3D::processMesh(aiMesh* mesh, const aiScene* scene, std::string path)
	{
		std::vector<GLfloat> vertexCoords;
		std::vector<GLfloat> texCoords;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> tangents;
		std::vector<GLuint> indices;
		std::vector<std::shared_ptr<Texture2D>> dTextures;
		std::vector<std::shared_ptr<Texture2D>> sTextures;
		
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			vertexCoords.push_back(mesh->mVertices[i].x);
			vertexCoords.push_back(mesh->mVertices[i].y);
			vertexCoords.push_back(mesh->mVertices[i].z);

			if (mesh->mTextureCoords[0])
			{
				texCoords.push_back(mesh->mTextureCoords[0][i].x);
				texCoords.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				texCoords.push_back(0.0f);
				texCoords.push_back(0.0f);
			}

			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);
			
			tangents.push_back(mesh->mTangents[i].x);
			tangents.push_back(mesh->mTangents[i].y);
			tangents.push_back(mesh->mTangents[i].z);
		}

		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			
			dTextures = loadMaterialTexture(material, aiTextureType_DIFFUSE, path);
			sTextures = loadMaterialTexture(material, aiTextureType_SPECULAR, path);
		}
		return std::make_shared<Mesh>(dTextures, sTextures, vertexCoords, texCoords, normals, tangents, indices);
	}

	std::vector<std::shared_ptr<Texture2D>> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string path)
	{
		std::vector<std::shared_ptr<Texture2D>> textures;
		for (size_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			path = std::string(path.substr(0, path.find_last_of('/')) + '/' + str.C_Str());
			std::string textureName = path.substr(path.find_last_of('/', path.find_last_of('/') - 1) + 1, path.length());

			std::shared_ptr<Texture2D> pTexture = ResourceManager::getTexture(textureName);

			if (!pTexture) 
			{
				pTexture = ResourceManager::loadTexture(textureName, path, true);
				if (pTexture)
				{
					std::cout << "---------->Load texture " << textureName << " successefully" << std::endl << std::endl;
				}
			}
			
			textures.push_back(pTexture);
		}
		return textures;
	}
}