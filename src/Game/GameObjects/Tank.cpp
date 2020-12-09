#include "Tank.h"
#include "../../Renderer/Sprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite,
		   const float velocity,
		   const glm::vec2& position,
		   const glm::vec2& size) :
	IGameObject(position, size, 0.0f),
	m_eOrientation(EOrientation::Up),
	m_pSprite(std::move(pSprite)),
	m_move(false),
	m_velocity(velocity),
	m_moveOffset(glm::vec2(0.0f, 1.0f))
{}
void Tank::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation);
}
void Tank::setOrientation(const EOrientation eOrientation)
{
	if (m_eOrientation == eOrientation) {
		return;
	}
	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case Tank::EOrientation::Up:
		m_moveOffset.x = 0.0f;
		m_moveOffset.y = 1.0f;
		break;
	case Tank::EOrientation::Left:
		m_moveOffset.x = -1.0f;
		m_moveOffset.y = 0.0f;
		break;
	case Tank::EOrientation::Down:
		m_moveOffset.x = 0.0f;
		m_moveOffset.y = -1.0f;
		break;
	case Tank::EOrientation::Right:
		m_moveOffset.x = 1.0f;
		m_moveOffset.y = 0.0f;
		break;
	}
}
void Tank::move(const bool move)
{
	m_move = move;
}
void Tank::update(const uint64_t delta)
{
	if (m_move)
	{
		m_position += delta * m_velocity * m_moveOffset;
		
		switch (m_eOrientation)
		{
		case Tank::EOrientation::Up:
			m_pSprite->startAnimationLooped("move_up");
			break;
		case Tank::EOrientation::Left:
			m_pSprite->startAnimationLooped("move_left");
			break;
		case Tank::EOrientation::Down:
			m_pSprite->startAnimationLooped("move_down");
			break;
		case Tank::EOrientation::Right:
			m_pSprite->startAnimationLooped("move_right");
			break;
		}
		m_pSprite->update(delta);
	}
	else
	{
		m_pSprite->stopAnimation();
	}
	
}
