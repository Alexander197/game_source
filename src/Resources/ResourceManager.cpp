#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap_t ResourceManager::m_shaderProgramsMap;
ResourceManager::TexturesMap_t ResourceManager::m_textures;
ResourceManager::SpritesMap_t ResourceManager::m_sprites;
std::string ResourceManager::m_path;
std::vector<std::vector<std::string>> ResourceManager::m_levels;

void ResourceManager::unloadAllResources()
{
	m_shaderProgramsMap.clear();
	m_textures.clear();
	m_sprites.clear();
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

std::shared_ptr<RenderEngine::ShaderProgram>ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string fragmentPath)
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

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderProgramsMap.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}
	std::cerr << "Can't load shader program\n"
		<< "Vartex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;
	return nullptr;
}
std::shared_ptr<RenderEngine::ShaderProgram>ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap_t::const_iterator it = m_shaderProgramsMap.find(shaderName);
	if (it != m_shaderProgramsMap.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}
std::shared_ptr<RenderEngine::Texture2D>  ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
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
	
	std::shared_ptr<RenderEngine::Texture2D> newTexture = 
		m_textures.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;
	
	stbi_image_free(pixels);
	return newTexture;
}
std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
	TexturesMap_t::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the texture: " << textureName << std::endl;
	return nullptr;
}
std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string& spriteName,
																  const std::string& textureName,
																  const std::string& shaderName,
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
	std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites.emplace(spriteName,
		std::make_shared<RenderEngine::Sprite>(pTexture, subTextureName, pShader)).first->second;
	
	return newSprite;
}
std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string& spriteName)
{
	SpritesMap_t::const_iterator it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the sprite: " << spriteName << std::endl;
	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,
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
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, 
								   static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);

			glm::vec2 rightBottomUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth,
									static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

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
bool ResourceManager::loadJSONResources(const std::string& JSONpath)
{
	const std::string JSONString = getFileString(JSONpath);
	if (JSONString.empty())
	{
		std::cerr << "No JSON resources file!" << std::endl;
		return false;
	}
	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << " (" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: "<< JSONpath <<std::endl;
		return false;
	}
	
	auto shadersIt = document.FindMember("shaders");
	if (shadersIt != document.MemberEnd())
	{
		const auto shaders = shadersIt->value.GetArray();
		for (const auto& currentShader : shaders)
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();
			loadShaders(name, filePath_v, filePath_f);
		}
	}

	auto textureAtlasesIt = document.FindMember("texureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		const auto textureAtlases = textureAtlasesIt->value.GetArray();
		for (const auto& currentTexureAtlas : textureAtlases)
		{
			const std::string name = currentTexureAtlas["name"].GetString();
			const std::string filePath = currentTexureAtlas["filePath"].GetString();

			const unsigned int subTextureWidth = currentTexureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeigth = currentTexureAtlas["subTextureHeigth"].GetUint();

			const auto subTexturesArray = currentTexureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			
			for (const auto& currentsubTextures : subTexturesArray)
			{
				subTextures.emplace_back(currentsubTextures.GetString());
			}
			loadTextureAtlas(name, filePath, subTextures, subTextureWidth, subTextureHeigth);
		}
	}

	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		const auto sprites = spritesIt->value.GetArray();
		for (const auto& currentSprite : sprites)
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTextureName = currentSprite["initialSubTexture"].GetString();

			auto pSprite = loadSprite(name, textureAtlas, shader, subTextureName);
			if (!pSprite)
			{ 
				continue;
			}

			auto animationsIt = currentSprite.FindMember("animations");
			if (animationsIt != currentSprite.MemberEnd())
			{
				const auto animations = animationsIt->value.GetArray();
				for (const auto& currentAnimation : animations)
				{
					const std::string animationName = currentAnimation["animationName"].GetString();
					const auto frames = currentAnimation["frames"].GetArray();

					std::vector<RenderEngine::Sprite::FrameDescription> framesDescriptions;
					framesDescriptions.reserve(frames.Size());
					for (const auto& currentFrame : frames)
					{
						const std::string subTexture = currentFrame["subTexture"].GetString();
						const uint64_t duration = currentFrame["duration"].GetUint64();

						const auto pTextureAtlas = getTexture(textureAtlas);
						const auto& pSubTexture = pTextureAtlas->getSubTexture(subTexture);

 						framesDescriptions.emplace_back(pSubTexture.leftBottomUV, pSubTexture.rightTopUV, duration);
					}
					pSprite->insertAnimation(std::move(animationName), std::move(framesDescriptions));
				}
			}
		}
	}

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
	{
		const auto levels = levelsIt->value.GetArray();
		for (const auto& currentLevel : levels)
		{
			const auto description = currentLevel["description"].GetArray();
			std::vector<std::string> levelRows;
			levelRows.reserve(description.Size());

			size_t maxLength = 0;

			for (const auto& currentRow : description)
			{
				levelRows.emplace_back(currentRow.GetString());
				if (maxLength < levelRows.back().length())
				{
					maxLength = levelRows.back().length();

				}
			}

			for (auto& currentRow : levelRows)
			{
				while (currentRow.length() < maxLength)
				{
					currentRow.append("D");
				}
			}
			m_levels.emplace_back(std::move(levelRows));
		}
	}

	return true;
}
