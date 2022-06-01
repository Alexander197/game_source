#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Model3D.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP

#include "stb_image.h"

ResourceManager::ShaderProgramsMap_t ResourceManager::m_shaderProgramsMap;
ResourceManager::TexturesMap_t ResourceManager::m_textures;
ResourceManager::SpritesMap_t ResourceManager::m_sprites;
ResourceManager::Models3DMap_t ResourceManager::m_models3D;

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

std::shared_ptr<RenderEngine::Model3D> ResourceManager::load3DModel(const std::string& modelName, 
																	const std::string& modelPath,
																	const std::string& textureName, 
																	const std::string& shaderName)
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
		std::cerr << "Can't find the shader program: " << shaderName << " for the 3D model" << std::endl;
		return nullptr;
	}

	std::vector<GLfloat> vertexCoords;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> normalsCoords;

	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> texCoordsIndices;
	std::vector<GLuint> normalsIndices;

	std::vector<GLfloat> tcI;

	std::string path = std::string(m_path + "/" + modelPath);
	std::ifstream in(std::string(m_path + "/" + modelPath).c_str());

	if (!in.is_open())
	{
		std::cerr << "Can't load model: " << modelPath << std::endl;
		return nullptr;
	}
	std::string line;
	double counter = 0;
	double sum = 0.0f;
	while (!in.eof())
	{
		std::getline(in, line);
		if (!line.empty())
		{
			if (line[0] == 'v' && line[1] == ' ')
			{
				float x = 0.0f, y = 0.0f, z = 0.0f;
				sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				vertexCoords.push_back(x);
				vertexCoords.push_back(y);
				vertexCoords.push_back(z);
			}
			else if (line[0] == 'v' && line[1] =='t')
			{
					float u = 0.0f, v = 0.0f;
					sscanf(line.c_str(), "vt %f %f", &u, &v);
					texCoords.push_back(u);
					texCoords.push_back(v);
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				float x = 0.0f, y = 0.0f, z = 0.0f;
				sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
				normalsCoords.push_back(x);
				normalsCoords.push_back(y);
				normalsCoords.push_back(z);
			}
			else if (line[0] == 'f' && line[1] == ' ')
			{
				
				int v1 = 0, v2 = 0, v3 = 0, t1 = 0, t2 = 0, t3 = 0, n1 = 0, n2 = 0, n3 = 0;
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				
				vertexIndices.push_back(v1 - 1);
				vertexIndices.push_back(v2 - 1);
				vertexIndices.push_back(v3 - 1);

				texCoordsIndices.push_back(t1 - 1);
				texCoordsIndices.push_back(t2 - 1);
				texCoordsIndices.push_back(t3 - 1);

				normalsIndices.push_back(n1 - 1);
				normalsIndices.push_back(n2 - 1);
				normalsIndices.push_back(n3 - 1);
			}
		}
	}
	std::vector<GLfloat> newVertices;
	std::vector<GLfloat> newTexCoords;
	std::vector<GLfloat> newNormalsCoords;
	std::vector<GLuint> newIndices;

	// 0 1 2 
	// 2 1 3
	// 1 4 3
	// 3 4 5

	size_t size = vertexIndices.size();

	for (size_t i = 0; i < size; i ++)
	{
		GLuint inV = vertexIndices[i];
		GLuint inT = texCoordsIndices[i];
		GLuint inN = normalsIndices[i];

		newVertices.push_back(vertexCoords[inV * 3]);
		newTexCoords.push_back(texCoords[inT * 2]);
		newNormalsCoords.push_back(normalsCoords[inN * 3]);

		newVertices.push_back(vertexCoords[inV * 3 + 1]);
		newTexCoords.push_back(texCoords[inT * 2 + 1]);
		newNormalsCoords.push_back(normalsCoords[inN * 3 + 1]);

		newVertices.push_back(vertexCoords[inV * 3 + 2]);
		//newTexCoords.push_back(texCoords[inT * 2 + 2]);
		newNormalsCoords.push_back(normalsCoords[inN * 3 + 2]);

		newIndices.push_back(i);
	}

	in.close();


	std::shared_ptr<RenderEngine::Model3D> newModel = m_models3D.emplace(modelName,
		std::make_shared<RenderEngine::Model3D>(pTexture, pShader, newVertices, newTexCoords, newNormalsCoords, newIndices)).first->second;

	return newModel;
}
std::shared_ptr<RenderEngine::Model3D> ResourceManager::get3DModel(const std::string& modelName)
{
	Models3DMap_t::const_iterator it = m_models3D.find(modelName);
	if (it != m_models3D.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the 3D model: " << modelName << std::endl;
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
					std::string animationName = currentAnimation["animationName"].GetString();
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

	/*auto modelsIt = document.FindMember("models");
	if (modelsIt != document.MemberEnd())
	{
		const auto models = modelsIt->value.GetArray();
		for (const auto& currentModel : models)
		{
			const std::string name = currentModel["name"].GetString();
			const std::string filePath = currentModel["filePath"].GetString();
			const std::string texture = currentModel["textureName"].GetString();
			const std::string shader = currentModel["shader"].GetString();
			auto model = load3DModel(name, filePath, texture, shader );
		}
	}*/

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
