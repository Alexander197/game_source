#pragma once
#include <glm/vec2.hpp>
#include <memory>
class BoundingBox
{
public:
	enum class Shape {
		NONE,
		CIRCLE,
		RECTANGLE_NON_ROTATED
	};
	BoundingBox(Shape shape, glm::vec2 center, float radius);
	BoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV);

	void setBoundingBox(Shape shape, glm::vec2 center, float radius);
	void setBoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV);

	static bool isColide(std::shared_ptr<BoundingBox> boundingBox1, std::shared_ptr<BoundingBox> boundingBox2);
	Shape getShape() { return m_shape; };

private:
	Shape m_shape;
	typedef std::pair<glm::vec2, float> CircleBox_t;
	CircleBox_t m_circleBox;

	typedef std::pair<glm::vec2, glm::vec2> RectangleBox_t;
	RectangleBox_t m_rectagleBox;
};