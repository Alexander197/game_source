#pragma once

#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

#include <memory>

namespace RenderEngine {
	class Sprite;
}

class BrickWall : public IGameObject {
public:
	BrickWall(std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer);
	virtual void render() const override;
	virtual void update(const double delta) override;

private:
	std::shared_ptr<RenderEngine::Sprite> m_pSprite;
};