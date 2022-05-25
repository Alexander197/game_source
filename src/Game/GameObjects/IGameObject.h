#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "BoundingBox.h"

class IGameObject {
public:
	IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const BoundingBox::Shape shape,
		const float layer, const glm::vec3 velocity = { 0.0f,0.0f,0.0f },
		const glm::vec3 aceleration = { 0.0f,0.0f,0.0f }, const float maxVelocity = 0);
	
	virtual void render() const = 0;
	virtual void update(const double delta) = 0;

	std::shared_ptr<BoundingBox> getBoundingBox() { return m_boundingBox; };

	void setVelocity(glm::vec3 velocity);
	void setLastPosition();
	void move(const bool move);
	
	virtual ~IGameObject();

protected:
	glm::vec3 m_position;
	glm::vec3 m_lastPosition;
	glm::vec2 m_size;
	float m_rotation;
	std::shared_ptr<BoundingBox> m_boundingBox;

	bool m_move;
	glm::vec3 m_velocity;
	float m_maxVelocity;
	glm::vec3 m_acceleration;

	float m_layer;
};