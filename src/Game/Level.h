#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/vec2.hpp>

class IGameObject;

class Level {
public:
	static constexpr unsigned int BLOCK_SIZE = 32;

	Level(const std::vector<std::string>& levelDescription);
	void render() const;
	void update(const double delta);

	size_t getLevelWidth() const;
	size_t getLevelHeight() const;

	glm::ivec2 getPlayerRespawn_1() const { return m_playerRespawn_1; }

private:
	size_t m_width;
	size_t m_height;

	glm::ivec2 m_playerRespawn_1;

	std::vector<std::shared_ptr<IGameObject>> m_mapObjects;
};