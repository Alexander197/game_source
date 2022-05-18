#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const BoundingBox::Shape shape,
	const float layer, const float velocity,
	const float aceleration, const float maxVelocity) : m_position(position),
									m_lastPosition(position),
									m_size(size),
									m_rotation(rotation),
									m_boundingBox(std::make_shared<BoundingBox>(shape, position, glm::vec2(position.x + size.x, position.y + size.y))),
									m_layer(layer),
									m_velocity(velocity),
									m_aceleration(aceleration),
									m_maxVelocity(maxVelocity),
									m_move(false)
{

}
void IGameObject::move(const bool move)
{
	m_move = move;
}
void IGameObject::setVelocity(float velocity)
{
	m_velocity = velocity;
}
void IGameObject::setLastPosition()
{
	m_position = m_lastPosition;
	m_boundingBox->setBoundingBox(BoundingBox::Shape::RECTANGLE_NON_ROTATED,
				glm::vec2(m_position.x, m_position.y),
				glm::vec2(m_position.x + m_size.x, m_position.y + m_size.y));
}
IGameObject::~IGameObject()
{

}