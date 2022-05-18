#pragma once

#include <glm/vec2.hpp>
#include "BoundingBox.h"

class IGameObject {
public:
	IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const BoundingBox::Shape shape,
		const float layer, const float velocity = 0,
		const float aceleration = 0, const float maxVelocity = 0);
	
	virtual void render() const = 0;
	virtual void update(const double delta) = 0;

	std::shared_ptr<BoundingBox> getBoundingBox() { return m_boundingBox; };

	void setVelocity(float velocity);
	void setLastPosition();
	void move(const bool move);
	
	virtual ~IGameObject();

protected:
	glm::vec2 m_position;
	glm::vec2 m_lastPosition;
	glm::vec2 m_size;
	float m_rotation;
	std::shared_ptr<BoundingBox> m_boundingBox;

	bool m_move;
	float m_velocity;
	float m_maxVelocity;
	float m_aceleration;

	float m_layer;
};