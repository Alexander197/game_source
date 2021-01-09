#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>
#include <string>
#include <map>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine {

	class Texture2D;
	class ShaderProgram;

	class Sprite {

	public:
		struct FrameDescription {
			FrameDescription(const glm::vec2 _leftBottonUV, const glm::vec2 _rightTopUV, uint64_t _duration) :
				leftBottomUV(_leftBottonUV),
				rightTopUV(_rightTopUV),
				duration(_duration)
			{}
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;
			uint64_t duration;
		};
		enum class EAnimationStates {
			None,
			Once,
			Looped
		};

		Sprite(std::shared_ptr<Texture2D> pTexture,
			   std::string initialSubTexture,
			   std::shared_ptr<ShaderProgram> pSharedProgram);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator = (const Sprite&) = delete;

		void render(const glm::vec2 position, const glm::vec2 size, const float rotation, const size_t frame_id = 0, const std::string& animationName = "", const float layer = 0.0f) const;

		void insertAnimation(const std::string& animationName, const std::vector<FrameDescription> framesDescriptions);
		std::vector<FrameDescription> getAnimation(const std::string& animationName) const;


	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;

		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_texCoordsBuffer;
		IndexBuffer m_indexBuffer;

		typedef std::map<const std::string, std::vector<FrameDescription>> AnimationsMap_t;
		AnimationsMap_t m_animations;
		
	};
}
