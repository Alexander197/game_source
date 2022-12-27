#pragma once
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace RenderEngine
{
	struct DirLigth {
		DirLigth(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
		{
			this->direction = direction;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
		}

		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	struct PointLight {
		PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
		{
			this->position = position;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
		}
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;
	};

	struct SpotLight {
		SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, 
			float linear, float quadratic, float cutOff, float outerCutOff)
		{
			this->position = position;
			this->direction = direction;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
			this->cutOff = cutOff;
			this->outerCutOff = outerCutOff;
		}
		glm::vec3 position;
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;
	};

	class Light
	{
	public:
		Light();
		void init();

		void addDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
		void addPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic);
		void addSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
			float linear, float quadratic, float cutOff, float outerCutOff);

		void addDirLight(std::shared_ptr<DirLigth> pDirLight);
		void addPointLight(std::shared_ptr<PointLight> pPointLight);
		void addSpotLight(std::shared_ptr<SpotLight> pSpotLight);

		std::vector<std::shared_ptr<DirLigth>> getDirLights() { return m_pDirLights; }
		std::vector<std::shared_ptr<PointLight>> getPointLighst() { return m_pPointLights; }
		std::vector<std::shared_ptr<SpotLight>> getSpotLights() { return m_pSpotLights; }

		void depthRender();
		void depthRender1();
		void bindDepthMap() const;

		glm::mat4 lightSpaceMatrix;
		glm::mat4 lightSpaceMatrix1;
	private:
		static std::vector<std::shared_ptr<DirLigth>> m_pDirLights;
		static std::vector<std::shared_ptr<PointLight>> m_pPointLights;
		static std::vector<std::shared_ptr<SpotLight>> m_pSpotLights;


		const unsigned int m_SHADOW_WIDTH;
		const unsigned int m_SHADOW_HEIGHT;
		GLuint m_depthMapFBO;
		GLuint m_depthMap;

		GLuint m_depthMapFBO1;
		GLuint m_depthMap1;
	};
}