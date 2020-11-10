#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap_t ResourceManager::m_shaderProgramsMap;
ResourceManager::TexturesMap_t ResourceManager::m_textures;
ResourceManager::SpritesMap_t ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap_t ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;

void ResourceManager::unloadAllResources()
{
	m_shaderProgramsMap.clear();
	m_textures.clear();
	m_sprites.clear();
	m_animatedSprites.clear();
}

void ResourceManager::setExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\	");
	m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		std::cerr << "Failed to open file" << relativeFilePath << std::endl;
		return std::string{};
	}
	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram>ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}
	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderProgramsMap.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	std::cerr << "Can't load shader program\n"
		<< "Vartex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;
	return nullptr;
}
std::shared_ptr<Renderer::ShaderProgram>ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap_t::const_iterator it = m_shaderProgramsMap.find(shaderName);
	if (it != m_shaderProgramsMap.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}
std::shared_ptr<Renderer::Texture2D>  ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);
	if (!pixels) 
	{
		std::cerr<<"Can't load image: "<< texturePath<<std::endl;
		return nullptr;
	}
	
	std::shared_ptr<Renderer::Texture2D> newTexture = 
		m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	
	stbi_image_free(pixels);
	return newTexture;
}
std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap_t::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}
std::shared_ptr<Renderer::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << " for the sprite" << std::endl;
		return nullptr;
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << " for the sprite" << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(spriteName,
		std::make_shared<Renderer::Sprite>(pTexture,
			subTextureName,
			pShader,
			glm::vec2(0.0f, 0.0f),
			glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}
std::shared_ptr<Renderer::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap_t::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName,
	const std::string& textureName,
	const std::string& shaderName,
	const unsigned int spriteWidth,
	const unsigned int spriteHeight,
	const std::string& subTextureName)
{
	auto pTexture = getTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << " for the animated sprite" << std::endl;
		return nullptr;
	}
	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader program: " << shaderName << " for the animated sprite" << std::endl;
		return nullptr;
	}
	std::shared_ptr<Renderer::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName,
		std::make_shared<Renderer::AnimatedSprite>(pTexture,
			subTextureName,
			pShader,
			glm::vec2(0.0f, 0.0f),
			glm::vec2(spriteWidth, spriteHeight))).first->second;
	return newSprite;
}
std::shared_ptr<Renderer::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string& spriteName)
{
	AnimatedSpritesMap_t::const_iterator it = m_animatedSprites.find(spriteName);
	if (it != m_animatedSprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the animated sprite: " << spriteName << std::endl;
	return nullptr;
}
std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,
	std::string texturePath,
	std::vector<std::string> subTextures,
	const unsigned int subTextureWidth,
	const unsigned int subTextureHeight)
{
	auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture)
	{
		const unsigned int textureWidth = pTexture->width();
		const unsigned int textureHeight = pTexture->height();

		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (auto& currentSubTextureName : subTextures)
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, 
								   static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);

			glm::vec2 rightBottomUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
									static_cast<float>(currentTextureOffsetY) / textureHeight);

			pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightBottomUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth)
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}

	}
	return pTexture;
}