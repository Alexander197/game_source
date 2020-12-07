#include "Tank.h"
#include "../../Renderer/Sprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite_up,
		   std::shared_ptr<RenderEngine::Sprite> pSprite_left,
		   std::shared_ptr<RenderEngine::Sprite> pSprite_down,
		   std::shared_ptr<RenderEngine::Sprite> pSprite_right,
		   const float velocity,
		   const glm::vec2& position,
		   const glm::vec2& size) :
	IGameObject(position, size, 0.0f),
	m_eOrientation(EOrientation::Up),
	m_pSprite_up(std::move(pSprite_up)),
	m_pSprite_left(std::move(pSprite_left)),
	m_pSprite_down(std::move(pSprite_down)),
	m_pSprite_right(std::move(pSprite_right)),
	m_spriteAnimator_up(m_pSprite_up),
	m_spriteAnimator_left(m_pSprite_left),
	m_spriteAnimator_down(m_pSprite_down),
	m_spriteAnimator_right(m_pSprite_right),
	m_move(false),
	m_velocity(velocity),
	m_moveOffset(glm::vec2(0.0f, 1.0f))
{}
void Tank::render() const
{
	switch (m_eOrientation)
	{
	case Tank::EOrientation::Up:
		m_pSprite_up->render(m_position, m_size, m_rotation, m_spriteAnimator_up.getCurrentFrame());
		break;
	case Tank::EOrientation::Left:
		m_pSprite_left->render(m_position, m_size, m_rotation, m_spriteAnimator_left.getCurrentFrame());
		break;
	case Tank::EOrientation::Down:
		m_pSprite_down->render(m_position, m_size, m_rotation, m_spriteAnimator_down.getCurrentFrame());
		break;
	case Tank::EOrientation::Right:
		m_pSprite_right->render(m_position, m_size, m_rotation, m_spriteAnimator_right.getCurrentFrame());
		break;
	}
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
			m_spriteAnimator_up.update(delta);
			break;
		case Tank::EOrientation::Left:
			m_spriteAnimator_left.update(delta);
			break;
		case Tank::EOrientation::Down:
			m_spriteAnimator_down.update(delta);
			break;
		case Tank::EOrientation::Right:
			m_spriteAnimator_right.update(delta);
			break;
		}
	}
}
