#include "Water.h"

Water::Water(std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer) :
	IGameObject(position, size, rotation, layer),
	m_pSprite(std::move(pSprite)),
	m_spriteAnimator(m_pSprite)
{
	m_spriteAnimator.startAnimationLooped("flow");
}
void Water::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator.getCurrentFrame(), m_spriteAnimator.getActiveAnimation());
}
void Water::update(const uint64_t delta)
{
	m_spriteAnimator.update(delta);
}
