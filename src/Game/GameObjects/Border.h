#pragma once

#include "IGameObject.h"

#include <memory>

namespace RenderEngine {
	class Sprite;
}

class Border : public IGameObject {
public:
	Border(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2 position, const glm::vec2 size, const float rotation, const float layer);
	virtual void render() const override;
	virtual void update(const uint64_t delta) override;

private:
	std::shared_ptr<RenderEngine::Sprite> m_pCurrentSprite;
};