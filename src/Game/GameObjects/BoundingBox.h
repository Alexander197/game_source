#pragma once
#include <glm/vec2.hpp>
#include <memory>
class BoundingBox
{
public:
	enum class Shape {
		NONE,
		CIRCLE,
		RECTANGLE_NON_ROTATED,
		TRIANGLE
	};
	struct MyStruct
	{
	public:
		MyStruct(Shape shape) : m_shape(shape) {};
	protected: 
		Shape m_shape;
	};

	struct Circle : public MyStruct
	{
		Circle(glm::vec2 center, float radius) : MyStruct(Shape::CIRCLE), m_center(center), m_radius(radius) {};
		glm::vec2 m_center;
		float m_radius;
	};
	struct RectangleNonRotated : public MyStruct
	{
		RectangleNonRotated(glm::vec2 leftBottom, glm::vec2 rightTop) : MyStruct(Shape::RECTANGLE_NON_ROTATED), 
				m_leftBottom(leftBottom),
				m_rightTop(rightTop){};
		glm::vec2 m_leftBottom;
		glm::vec2 m_rightTop;
	};
	struct Triangle : public MyStruct
	{
		Triangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) : MyStruct(Shape::TRIANGLE),
			m_v1(v1),
			m_v2(v2),
			m_v3(v3) {};
		glm::vec2 m_v1;
		glm::vec2 m_v2;
		glm::vec2 m_v3;
	};

	BoundingBox(Shape shape, glm::vec2 center, float radius);
	BoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV);

	void setBoundingBox(Shape shape, glm::vec2 center, float radius);
	void setBoundingBox(Shape shape, glm::vec2 leftBottomUV, glm::vec2 rightTopUV);
	void setBoundingBox(glm::vec2 position, glm::vec2 size);

	static bool isColide(std::shared_ptr<BoundingBox> boundingBox1, std::shared_ptr<BoundingBox> boundingBox2);
	Shape getShape() { return m_shape; };

private:
	Shape m_shape;
	typedef std::pair<glm::vec2, float> CircleBox_t;
	CircleBox_t m_circleBox;

	typedef std::pair<glm::vec2, glm::vec2> RectangleBox_t;
	RectangleBox_t m_rectagleBox;
};