#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const BoundingBox::Shape shape,
	const float layer, glm::vec3 velocity, const glm::vec3 aceleration,
	const float maxVelocity) : m_position(glm::vec3(position, 0.0f)),
									m_lastPosition(glm::vec3(position, 0.0f)),
									m_size(size),
									m_rotation(rotation),
									m_boundingBox(std::make_shared<BoundingBox>(shape, position, glm::vec2(position.x + size.x, position.y + size.y))),
									m_layer(layer),
									m_velocity(velocity),
									m_acceleration(aceleration),
									m_maxVelocity(maxVelocity),
									m_move(false)
{

}
void IGameObject::move(const bool move)
{
	m_move = move;
}
void IGameObject::setVelocity(glm::vec3 velocity)
{
	m_velocity = velocity;
}
void IGameObject::setLastPosition()
{
	//m_velocity = { 0.0f,0.0f,0.0f };
	m_position = m_lastPosition;
	//m_boundingBox->setBoundingBox(BoundingBox::Shape::RECTANGLE_NON_ROTATED,
		//		glm::vec2(m_position.x, m_position.y),
			//	glm::vec2(m_position.x + m_size.x, m_position.y + m_size.y));
	m_boundingBox->setBoundingBox(m_position + glm::vec3(m_size / 12.0f, 0.0f), m_size / 1.2f);
}
IGameObject::~IGameObject()
{

}