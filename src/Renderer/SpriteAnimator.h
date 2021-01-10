#pragma once

#include "Sprite.h"

#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace RenderEngine {
	class Sprite;

	class SpriteAnimator {
	public:
		enum class EAnimationStates {
			None,
			Once,
			Looped
		};

		SpriteAnimator(std::shared_ptr<Sprite> pSprite);

		std::string getActiveAnimation() const { return m_activeAnimation; }
		size_t getCurrentFrame() const { return m_currentFrame; }

		void update(const double delta);

		bool startAnimationLooped(const std::string& activeAnimation);

		bool startAnimationOnce(const std::string& activeAnimation);
		bool startAnimationOnce(const std::string& activeAnimation, std::function<void()> callback);

		void stopAnimation();

	private:
		std::shared_ptr<Sprite> m_pSprite;
		std::string m_activeAnimation;
		std::vector<Sprite::FrameDescription> m_activeAnimationDescription;

		EAnimationStates m_currentAnimationState;
		size_t m_currentFrame;
		double m_currentFrameDuration;
		double m_currentAnimationTime;

		std::function<void()> m_callback;
		bool m_isCallback;
	};
}