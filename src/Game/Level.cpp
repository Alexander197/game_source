#include "Level.h"
#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWall.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Bush.h"
#include "GameObjects/Border.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

std::shared_ptr<IGameObject> createGameObjectFromDescription(const char description, const glm::vec2 position, const glm::vec2 size, const float rotation = 0) 
{
	switch (description)
	{
	case '0':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_right"), position, size, rotation, 0.0f);
		break;
	case '1':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_bottom"), position, size, rotation, 0.0f);
		break;
	case '2':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_left"), position, size, rotation, 0.0f);
		break;
	case '3':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_top"), position, size, rotation, 0.0f);
		break;
	case '4':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_full"), position, size, rotation, -1.0f);
		break;
	case 'A':
		return std::make_shared<Eagle>(ResourceManager::getSprite("eagle"), position, size, rotation, 1.0f);
		break;
	case 'C':
		return std::make_shared<BetonWall>(ResourceManager::getSprite("beton_full"), position, size, rotation, 0.0f);
		break;
	case 'E':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("steel"), position, size, rotation, 0.0f);
		break;
	case 'F':
		return std::make_shared<Water>(ResourceManager::getSprite("water"), position, size, rotation, 0.0f);
		break;
	case 'G':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_left_bottom"), position, size, rotation, 0.0f);
		break;
	case 'H':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_right_bottom"), position, size, rotation, 0.0f);
		break;
	case 'I':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_left_top"), position, size, rotation, 0.0f);
		break;
	case 'J':
		return std::make_shared<BrickWall>(ResourceManager::getSprite("brick_right_top"), position, size, rotation, 0.0f);
		break;
	case 'K':
		return std::make_shared<Bush>(ResourceManager::getSprite("bush"), position, size, rotation, 1.0f);
		break;
	case 'D':
		return nullptr;
		break;
	default:
		std::cerr << "Unknown game object description: " << description << std::endl;
		break;
	}
	return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cerr << "Empty level description" << std::endl;
	}

	m_width = levelDescription[0].length();
	m_height = levelDescription.size();

	m_playerRespawn_1 = {32.0f, 16.0f};

	m_mapObjects.reserve(m_width * m_height + 4);

	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (m_height - 1) + BLOCK_SIZE / 2.0f);
	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		for (const char currentElement : currentRow)
		{
			switch (currentElement)
			{
			case 'V':
				m_playerRespawn_1 = { currentLeftOffset, currentBottomOffset };
				break;
			default:
				m_mapObjects.emplace_back(createGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE)));
				break;
			}
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}

	m_mapObjects.emplace_back(std::make_shared<Border>(ResourceManager::getSprite("border"), glm::vec2(BLOCK_SIZE,0.0f), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.0f), 0.0f, 0.0f)); //Bottom

	m_mapObjects.emplace_back(std::make_shared<Border>(ResourceManager::getSprite("border"), glm::vec2(BLOCK_SIZE, BLOCK_SIZE * (m_height + 0.5f)), glm::vec2(m_width * BLOCK_SIZE, BLOCK_SIZE / 2.0f), 0.0f, 0.0f)); //Top

	m_mapObjects.emplace_back(std::make_shared<Border>(ResourceManager::getSprite("border"), glm::vec2(0.0f, 0.0f), glm::vec2(BLOCK_SIZE, BLOCK_SIZE * (m_height + 1)), 0.0f, 0.0f)); //Left

	m_mapObjects.emplace_back(std::make_shared<Border>(ResourceManager::getSprite("border"), glm::vec2(BLOCK_SIZE * (m_width + 1), 0.0f), glm::vec2(BLOCK_SIZE * 2.0f, BLOCK_SIZE * (m_height + 1)), 0.0f, 0.0f)); //Right
}
void Level::render() const
{
	for (const auto& currentMapObject : m_mapObjects)
	{
		if (currentMapObject)
		{
			currentMapObject->render();
		}
	}
}
void Level::update(const double delta)
{
	for (const auto& currentMapObject : m_mapObjects)
	{
		if (currentMapObject)
		{
			currentMapObject->update(delta);
		}
	}
}
std::vector<std::shared_ptr<IGameObject>> Level::getMapObjects()
{
	return m_mapObjects;
}

size_t Level::getLevelWidth() const
{
	return (m_width + 3) * BLOCK_SIZE;
}
size_t Level::getLevelHeight() const
{
	return (m_height + 1) * BLOCK_SIZE;
}