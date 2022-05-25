#include "BoundingBox.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

BoundingBox::BoundingBox(Shape shape, glm::vec2 center, float radius) : m_shape(shape),
																		m_circleBox(std::make_pair(center, radius))
{

}
BoundingBox::BoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV) : m_shape(shape),
																					  m_rectagleBox(std::make_pair(leftBottomUV, rightTopUV))
{

}

void BoundingBox::setBoundingBox(Shape shape, glm::vec2 center, float radius)
{
	m_shape = shape;
	m_circleBox = std::make_pair(center, radius);
}

void BoundingBox::setBoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV)
{
	m_shape = shape;
	m_rectagleBox = std::make_pair(leftBottomUV, rightTopUV);
}
void BoundingBox::setBoundingBox(glm::vec2 position, glm::vec2 size)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

	glm::vec4 leftBottom = modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 rightTop = modelMatrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

	m_rectagleBox = std::make_pair(leftBottom, rightTop);
}

bool BoundingBox::isColide(std::shared_ptr<BoundingBox> boundingBox1, std::shared_ptr<BoundingBox> boundingBox2)
{
	if (boundingBox1->m_shape == Shape::NONE || boundingBox2->m_shape == Shape::NONE)
	{
		return false;
	}
	else if (boundingBox1->m_shape == Shape::CIRCLE && boundingBox2->m_shape == Shape::CIRCLE)
	{
		float distance = glm::distance(boundingBox1->m_circleBox.first, boundingBox2->m_circleBox.first);
		return distance < (boundingBox1->m_circleBox.second + boundingBox2->m_circleBox.second);
	}
	else if (boundingBox1->m_shape == Shape::RECTANGLE_NON_ROTATED && boundingBox2->m_shape == Shape::RECTANGLE_NON_ROTATED)
	{
		float center1x = (boundingBox1->m_rectagleBox.first.x + boundingBox1->m_rectagleBox.second.x) / 2;
		float length1x = (boundingBox1->m_rectagleBox.second.x - boundingBox1->m_rectagleBox.first.x);

		float center2x = (boundingBox2->m_rectagleBox.first.x + boundingBox2->m_rectagleBox.second.x) / 2;
		float length2x = (boundingBox2->m_rectagleBox.second.x - boundingBox2->m_rectagleBox.first.x);

		float center1y = (boundingBox1->m_rectagleBox.first.y + boundingBox1->m_rectagleBox.second.y) / 2;
		float length1y = (boundingBox1->m_rectagleBox.second.y - boundingBox1->m_rectagleBox.first.y);

		float center2y = (boundingBox2->m_rectagleBox.first.y + boundingBox2->m_rectagleBox.second.y) / 2;
		float length2y = (boundingBox2->m_rectagleBox.second.y - boundingBox2->m_rectagleBox.first.y);
		
		return std::abs(center1x - center2x) < (length1x + length2x) / 2 && std::abs(center1y - center2y) < (length1y + length2y) / 2;
	}
	return false;
}