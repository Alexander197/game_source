#include "Light.h"
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	std::vector<std::shared_ptr<DirLigth>> Light::m_pDirLights;
	std::vector<std::shared_ptr<PointLight>> Light::m_pPointLights;
	std::vector<std::shared_ptr<SpotLight>> Light::m_pSpotLights;

	Light::Light() : m_SHADOW_WIDTH(4096), m_SHADOW_HEIGHT(4096)
	{ }
	void Light::init()
	{
		glGenFramebuffers(1, &m_depthMapFBO);

		glGenTextures(1, &m_depthMap);
		glBindTexture(GL_TEXTURE_2D, m_depthMap);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_SHADOW_WIDTH, m_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenFramebuffers(1, &m_depthMapFBO1);

		glGenTextures(1, &m_depthMap1);
		glBindTexture(GL_TEXTURE_2D, m_depthMap1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_SHADOW_WIDTH, m_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap1, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Light::addDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		m_pDirLights.push_back(std::make_shared<DirLigth>(direction, ambient, diffuse, specular));
	}
	void Light::addPointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
	{
		m_pPointLights.push_back(std::make_shared<PointLight>(position, ambient, diffuse, specular, constant, linear, quadratic));
	}
	void Light::addSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
		float linear, float quadratic, float cutOff, float outerCutOff)
	{
		m_pSpotLights.push_back(std::make_shared<SpotLight>(position, direction, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff));
	}

	void Light::addDirLight(std::shared_ptr<DirLigth> pDirLight)
	{
		m_pDirLights.push_back(pDirLight);
	}
	void Light::addPointLight(std::shared_ptr<PointLight> pPointLight)
	{
		m_pPointLights.push_back(pPointLight);
	}
	void Light::addSpotLight(std::shared_ptr<SpotLight> pSpotLight)
	{
		m_pSpotLights.push_back(pSpotLight);
	}

	void Light::depthRender()
	{
		glViewport(0, 0, m_SHADOW_WIDTH, m_SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		float nearPlane = 1.0f, farPlane = 50.0f;
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane);
		//glm::mat4 lightProjection = glm::perspective(glm::radians(45.0f), 1.92f, 5.0f, 50.0f);
		//glm::vec3 lightDir = glm::normalize(m_pDirLights[0]->direction) * 10.0f;
		glm::mat4 lightView = glm::lookAt(m_pDirLights[0]->direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
	}
	void Light::depthRender1()
	{
		glViewport(0, 0, m_SHADOW_WIDTH, m_SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO1);
		glClear(GL_DEPTH_BUFFER_BIT);

		float nearPlane = 1.0f, farPlane = 50.0f;
		glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, nearPlane, farPlane);
		//glm::mat4 lightProjection = glm::perspective(glm::radians(60.0f), 1.92f, 1.0f, 100.0f);
		//glm::vec3 lightDir = glm::normalize(m_pDirLights[0]->direction) * 10.0f;
		glm::mat4 lightView = glm::lookAt(m_pDirLights[1]->direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix1 = lightProjection * lightView;
	}
	void Light::bindDepthMap() const
	{
		glBindTexture(GL_TEXTURE_2D, m_depthMap);
	}
}