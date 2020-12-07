#pragma once

#include <glm/vec2.hpp>

#include <memory>

#include "IGameObject.h"
#include "../../Renderer/SpriteAnimator.h"

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

	Tank(std::shared_ptr<RenderEngine::Sprite> pSprite_up,
		 std::shared_ptr<RenderEngine::Sprite> pSprite_left,
		 std::shared_ptr<RenderEngine::Sprite> pSprite_down,
		 std::shared_ptr<RenderEngine::Sprite> pSprite_right,
		 const float velocity,
		 const glm::vec2& position,
		 const glm::vec2& size);

	void render() const override;
	void setOrientation(const EOrientation eOrientation);
	void move(const bool move);
	void update(const uint64_t delta) override;

private:
	EOrientation m_eOrientation;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_up;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_left;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_down;
	std::shared_ptr<RenderEngine::Sprite> m_pSprite_right;

	RenderEngine::SpriteAnimator m_spriteAnimator_up;
	RenderEngine::SpriteAnimator m_spriteAnimator_left;
	RenderEngine::SpriteAnimator m_spriteAnimator_down;
	RenderEngine::SpriteAnimator m_spriteAnimator_right;

	bool m_move;
	float m_velocity;
	glm::vec2 m_moveOffset;
};