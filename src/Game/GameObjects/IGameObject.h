#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "AxisAlignedBoundingBox.h"

class IGameObject {
public:
	IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer,
		const float scale = 1.0f,
		const bool isTransparent = false,
		const glm::vec2 velocity = glm::vec2{ 0.0f,0.0f },
		const glm::vec2 aceleration = glm::vec2{ 0.0f,0.0f },
		const float maxVelocity = 0);
	
	virtual void render() const = 0;
	virtual void update(const double delta) = 0;

	std::shared_ptr<AxisAlignedBoundingBox> getBoundingBox() { return m_pBoundingBox; };

	void setVelocity(glm::vec3 velocity);
	void setLastPosition();
	void move(const bool move);
	
	virtual ~IGameObject();

protected:
	glm::vec2 m_position;
	glm::vec2 m_lastPosition;
	glm::vec2 m_size;
	float m_rotation;
	std::shared_ptr<AxisAlignedBoundingBox> m_pBoundingBox;

	bool m_move;
	glm::vec2 m_velocity;
	float m_maxVelocity;
	glm::vec2 m_acceleration;

	float m_layer;
};