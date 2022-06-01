#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer,
	const float scale,
	const bool isTransparent,
	glm::vec2 velocity, const glm::vec2 aceleration,
	const float maxVelocity) : m_position(position),
							m_lastPosition(position),
							m_size(size),
							m_rotation(rotation),
							m_pBoundingBox(std::make_shared<AxisAlignedBoundingBox>(position, size, scale, isTransparent)),
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
	m_position = m_lastPosition;
	m_pBoundingBox->setBoundingBox(m_position, m_size);
}
IGameObject::~IGameObject()
{

}