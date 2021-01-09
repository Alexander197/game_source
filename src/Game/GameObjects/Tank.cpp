#include "Tank.h"

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite,
		   const float velocity,
		   const glm::vec2& position,
		   const glm::vec2& size,
		   const float layer) :
	IGameObject(position, size, 0.0f, layer),
	m_eOrientation(EOrientation::Up),
	m_pSprite(std::move(pSprite)),
	m_spriteAnimator(m_pSprite),
	m_move(false),
	m_velocity(velocity),
	m_moveOffset(glm::vec2(0.0f, 1.0f))
{}
void Tank::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator.getCurrentFrame(), m_spriteAnimator.getActiveAnimation());
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
void Tank::update(const double delta)
{
	if (m_move)
	{
		m_position += static_cast<float>(delta) * m_velocity * m_moveOffset;
		
		switch (m_eOrientation)
		{
		case Tank::EOrientation::Up:
			m_spriteAnimator.startAnimationLooped("move_up");
			break;
		case Tank::EOrientation::Left:
			m_spriteAnimator.startAnimationLooped("move_left");
			break;
		case Tank::EOrientation::Down:
			m_spriteAnimator.startAnimationLooped("move_down");
			break;
		case Tank::EOrientation::Right:
			m_spriteAnimator.startAnimationLooped("move_right");
			break;
		}
		m_spriteAnimator.update(delta);
	}
	else
	{
		m_spriteAnimator.stopAnimation();
	}
	
}
