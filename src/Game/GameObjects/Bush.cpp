#include "Bush.h"

Bush::Bush(std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer) :
	IGameObject(position, size, rotation, layer),
	m_pSprite(std::move(pSprite))
{

}
void Bush::render() const
{
	m_pSprite->render(m_position, m_size, m_rotation, m_layer);
}
void Bush::update(const double delta)
{

}
