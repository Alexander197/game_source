#pragma once

#include <glm/vec2.hpp>

#include <memory>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../Renderer/Sprite.h"
#include "../../System/Timer.h"

namespace RenderEngine {
	class Sprite;
}

class Tank : public IGameObject{
public:
	enum class EOrientation {
		Up,
		Left,
		Down,
		Right
	};

	Tank(std::shared_ptr<RenderEngine::Sprite> pSprite,
		const float velocity,
		const float aceleration,
		const float maxVelocity,
		const glm::vec2& position,
		const glm::vec2& size,
		const float layer);

	void render() const override;
	void setOrientation(const EOrientation eOrientation);
	void update(const double delta) override;

private:
	EOrientation m_eOrientation;

	std::shared_ptr<RenderEngine::Sprite> m_pTankSprite;
	std::shared_ptr<RenderEngine::Sprite> m_pShieldSprite;

	RenderEngine::SpriteAnimator m_tankSpriteAnimator;
	RenderEngine::SpriteAnimator m_shieldSpriteAnimator;

	glm::vec2 m_moveOffset;

	bool m_isSpawning;
	bool m_hasShield;
	Timer m_spawnTimer;
	Timer m_shieldTimer;
};