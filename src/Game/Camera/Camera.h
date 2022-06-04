#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(glm::vec3 position);

	glm::vec3 getPosition() { return m_cameraPosition; }
	glm::mat4 getLookAtMatrix();
	float getFov() { return m_fov; }

	void setPitch(float offset, float delta);
	void setYaw(float offset, float delta);
	void setRoll(float offset, float delta);

	void setFov(float offset, float delta);

	void moveForward(const float delta);
	void moveLeft(const float delta);
	void moveBackward(const float delta);
	void moveRight(const float delta);
	void moveUp(const float delta);
	void moveDown(const float delta);

private:
	glm::vec3 m_cameraPosition;

	glm::vec3 m_front;
	glm::vec3 m_up;

	float m_pitch;
	float m_yaw;
	float m_roll;

	float m_fov;

	float m_speed;
	glm::vec3 m_sensitivity;
};