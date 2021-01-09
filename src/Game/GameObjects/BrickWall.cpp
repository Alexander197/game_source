#include "BrickWall.h"

#include "../../Renderer/Sprite.h"

BrickWall::BrickWall(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer) :
	IGameObject(position, size, rotation, layer),
	m_pCurrentSprite(pSprite)
{

}
void BrickWall::render() const
{
	m_pCurrentSprite->render(m_position, m_size, m_rotation, m_layer);
}
void BrickWall::update(const uint64_t delta)
{

}
