#include "AxisAlignedBoundingBox.h"
#include <glm/vec2.hpp>

AxisAlignedBoundingBox::AxisAlignedBoundingBox(glm::vec2 position, glm::vec2 size, float scale, bool isTransparent) :
	m_position(position), m_size(size), m_scale(scale), m_isTransparent(isTransparent)
{ }
void AxisAlignedBoundingBox::setBoundingBox(const glm::vec2 position, const glm::vec2 size)
{
	m_size = size * m_scale;
	m_position = position + size * (1.0f - 1.0f * m_scale) / 2.0f;
}

int AxisAlignedBoundingBox::isCollide(const std::shared_ptr<AxisAlignedBoundingBox> aabb1, const std::shared_ptr<AxisAlignedBoundingBox> aabb2)
{
	if (aabb1->m_isTransparent || aabb2->m_isTransparent) return 2;
	bool XColl = (aabb1->m_position.x + aabb1->m_size.x >= aabb2->m_position.x) && (aabb1->m_position.x <= aabb2->m_position.x + aabb2->m_size.x);
	bool YColl = (aabb1->m_position.y + aabb1->m_size.y >= aabb2->m_position.y) && (aabb1->m_position.y <= aabb2->m_position.y + aabb2->m_size.y);
	return XColl && YColl;
}