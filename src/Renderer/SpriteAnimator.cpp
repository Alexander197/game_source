#include "SpriteAnimator.h"

namespace RenderEngine {
	SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite) :
		m_pSprite(std::move(pSprite)),
		m_currentFrame(0),
		m_currentAnimationTime(0),
		m_currentFrameDuration(0),
		m_currentAnimationState(EAnimationStates::None)
	{

	}
	void SpriteAnimator::update(const uint64_t delta)
	{
		if (m_activeAnimationDescription.size() != 0)
		{
			m_currentAnimationTime += delta;

			while (m_currentAnimationTime >= m_currentFrameDuration)
			{
				m_currentAnimationTime -= m_currentFrameDuration;
				++m_currentFrame;

				if (m_currentFrame == m_activeAnimationDescription.size())
				{
					m_currentFrame = 0;
				}
				m_currentFrameDuration = m_activeAnimationDescription[m_currentFrame].duration;
			}
		}
	}

	bool SpriteAnimator::startAnimationLooped(const std::string& activeAnimation)
	{
		if (m_activeAnimation != activeAnimation || m_currentAnimationState != EAnimationStates::Looped)
		{
			m_activeAnimation = activeAnimation;
			auto framesDescription = m_pSprite->getAnimation(activeAnimation);
			if (framesDescription.size() != 0)
			{
				m_activeAnimationDescription = framesDescription;
				m_currentFrame = 0;
				m_currentAnimationTime = 0;
				m_currentFrameDuration = framesDescription[0].duration;
				m_currentAnimationState = EAnimationStates::Looped;
				return true;
			}
		}
		return false;
	}
	bool SpriteAnimator::startAnimationOnce(const std::string& activeAnimation)
	{
		m_activeAnimation = activeAnimation;
		auto framesDescription = m_pSprite->getAnimation(activeAnimation);
		if (framesDescription.size() != 0)
		{
			m_currentFrame = 0;
			m_currentAnimationTime = 0;
			m_currentFrameDuration = framesDescription[0].duration;
			m_currentAnimationState = EAnimationStates::Once;
			return true;
		}
		return false;
	}
	void SpriteAnimator::stopAnimation()
	{
		if (m_currentAnimationState != EAnimationStates::None)
		{
			m_currentAnimationState = EAnimationStates::None;
		}
	}
}
