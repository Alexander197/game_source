#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer, const float velocity,
	const float aceleration, const float maxVelocity) : m_position(position),
									m_size(size),
									m_rotation(rotation),
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
IGameObject::~IGameObject()
{

}