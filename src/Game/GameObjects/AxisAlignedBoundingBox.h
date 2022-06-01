#pragma once
#include <glm/vec2.hpp>
#include <memory>
class AxisAlignedBoundingBox
{
public:

	AxisAlignedBoundingBox(glm::vec2 position, glm::vec2 size, float scale = 1.0f, bool isTransparent = false);
	void setBoundingBox(const glm::vec2 position, const glm::vec2 size);

	static int isCollide(const std::shared_ptr<AxisAlignedBoundingBox> aabb1, const std::shared_ptr<AxisAlignedBoundingBox> aabb2);

private:
	glm::vec2 m_position;
	glm::vec2 m_size;
	float m_scale;

	bool m_isTransparent;
};