#pragma once

#include <glm/vec2.hpp>

#include <memory>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../Renderer/Sprite.h"

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
		 const glm::vec2& position,
		 const glm::vec2& size,
		 const float layer);

	void render() const override;
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move);
	void update(const double delta) override;

private:
	EOrientation m_eOrientation;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
	RenderEngine::SpriteAnimator m_spriteAnimator;

	bool m_move;
	float m_velocity;
	glm::vec2 m_moveOffset;
};