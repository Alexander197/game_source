#include "Tank.h"
#include "../../Resources/ResourceManager.h"
#include <glm/glm.hpp>

Tank::Tank(std::shared_ptr<RenderEngine::Sprite> pSprite,
	const float velocity,
	const float aceleration,
	const float maxVelocity,
	const glm::vec2& position,
	const glm::vec2& size,
	const float layer) :
	IGameObject(position, size, 0.0f, BoundingBox::Shape::RECTANGLE_NON_ROTATED, layer, velocity, aceleration, maxVelocity),
	m_eOrientation(EOrientation::Up),
	m_pTankSprite(std::move(pSprite)),
	m_pShieldSprite(ResourceManager::getSprite("shield")),
	m_tankSpriteAnimator(m_pTankSprite),
	m_shieldSpriteAnimator(m_pShieldSprite),
	m_moveOffset(glm::vec2(0.0f, 1.0f)),
	m_isSpawning(true),
	m_hasShield(false)
{
	m_spawnTimer.start(4000);
	m_tankSpriteAnimator.startAnimationLooped("respawn");
	m_spawnTimer.setCallback([&]()
		{
			m_isSpawning = false;
			m_hasShield = true;
			m_shieldSpriteAnimator.startAnimationLooped("shield");
			m_tankSpriteAnimator.startAnimationLooped("spawned");
			m_shieldTimer.start(4000);
		}
	);
	m_shieldTimer.setCallback([&]()
		{
			m_shieldSpriteAnimator.stopAnimation();
			m_hasShield = false;
		}
	);
}
void Tank::render() const
{
	m_pTankSprite->render(m_position, m_size, m_rotation, m_layer, m_tankSpriteAnimator.getCurrentFrame(), m_tankSpriteAnimator.getActiveAnimation());
	if (m_hasShield)
	{
		m_pShieldSprite->render(m_position, m_size, m_rotation, m_layer + 1.0f, m_shieldSpriteAnimator.getCurrentFrame(), m_shieldSpriteAnimator.getActiveAnimation());
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
void Tank::update(const double delta)
{
	if (!m_isSpawning)
	{
		if (m_move)
		{
			m_lastPosition = m_position;
			m_velocity += static_cast<float>(delta) * m_aceleration * ((m_maxVelocity - m_velocity) / m_maxVelocity);
			m_position += static_cast<float>(delta) * m_velocity * m_moveOffset;

			m_boundingBox->setBoundingBox(BoundingBox::Shape::RECTANGLE_NON_ROTATED,
				glm::vec2(m_position.x, m_position.y),
				glm::vec2(m_position.x + m_size.x, m_position.y + m_size.y));

			switch (m_eOrientation)
			{
			case Tank::EOrientation::Up:
				m_tankSpriteAnimator.startAnimationLooped("move_up");
				break;
			case Tank::EOrientation::Left:
				m_tankSpriteAnimator.startAnimationLooped("move_left");
				break;
			case Tank::EOrientation::Down:
				m_tankSpriteAnimator.startAnimationLooped("move_down");
				break;
			case Tank::EOrientation::Right:
				m_tankSpriteAnimator.startAnimationLooped("move_right");
				break;
			}
		}
		else
		{
			m_velocity = 0.0f;
			m_tankSpriteAnimator.stopAnimation();
		}
	}

	m_tankSpriteAnimator.update(delta);
	m_shieldSpriteAnimator.update(delta);
	m_spawnTimer.update(delta);
	m_shieldTimer.update(delta);
}