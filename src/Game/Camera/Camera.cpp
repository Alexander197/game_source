#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) : m_cameraPosition(position)
{
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_pitch = 0.0f;
	m_yaw = 0.0f;
	m_roll = 0.0;

	m_fov = 60.0f;

	m_speed = 0.008;
	m_sensitivity = glm::vec3(0.02, 0.02, 0.05);
}

glm::mat4 Camera::getLookAtMatrix()
{
	float x = cos(glm::radians(m_yaw - 90.0f)) * cos(glm::radians(m_pitch));
	float y = sin(glm::radians(m_pitch));
	float z = sin(glm::radians(m_yaw - 90.f)) * cos(glm::radians(m_pitch));
	m_front = glm::vec3(x, y, z);
	glm::vec3 lookAtPos = m_cameraPosition + m_front;

	//m_up = glm::vec3(-sin(glm::radians(m_roll)), cos(glm::radians(m_roll)), 0.0f);
	return glm::lookAt(m_cameraPosition, lookAtPos, m_up);
}

void Camera::setSpeed(float speed)
{
	m_speed = speed;
}

void Camera::setPitch(float offset, float delta)
{
	m_pitch += offset * m_sensitivity.y * delta;
	//m_pitch += offset * cos(glm::radians(m_roll)) * m_sensitivity.y * delta;
	//m_yaw -= offset * sin(glm::radians(m_roll)) * m_sensitivity.x * delta;
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	else if (m_pitch < -89.0f)
		m_pitch = -89.0f;
}
void Camera::setYaw(float offset, float delta)
{
	m_yaw += offset * m_sensitivity.x * delta;
	//m_yaw += offset * cos(glm::radians(m_roll)) * m_sensitivity.x * delta;
	//m_pitch += offset * sin(glm::radians(m_roll)) * m_sensitivity.y * delta;
}
void Camera::setRoll(float offset, float delta)
{
	m_roll += offset * m_sensitivity.z * delta;
}

void Camera::setFov(float offset, float delta)
{
	m_fov += offset * delta;
	if (m_fov < 1.0f)
	{
		m_fov = 1.0f;
	}
	else if (m_fov > 90.0f)
	{
		m_fov = 90.0f;
	}
}

void Camera::moveForward(const float delta)
{
	m_cameraPosition += m_front * m_speed * delta;
}
void Camera::moveLeft(const float delta)
{
	m_cameraPosition += glm::normalize(glm::cross(m_up, m_front)) * m_speed * delta;
}
void Camera::moveBackward(const float delta)
{
	m_cameraPosition -= m_front * m_speed * delta;
}
void Camera::moveRight(const float delta)
{
	m_cameraPosition -= glm::normalize(glm::cross(m_up, m_front)) * m_speed * delta;
}
void Camera::moveUp(const float delta)
{
	m_cameraPosition += m_up * m_speed * delta;
}
void Camera::moveDown(const float delta)
{
	m_cameraPosition -= m_up * m_speed * delta;
}