# pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>

#include "../Renderer/Model3D.h"
#include "../Renderer/Light.h"

class Tank;
class Level;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const double delta);
	
	void setKey(const int key, const int action);
	void setMouseButton(const int button, const int action);
	void setCursorPos(const double xpos, const double ypos);
	void setScrollOffset(const double xoffset, const double yoffset);
	
	
	bool init();

	size_t getCurrentLevelWidth() const;
	size_t getCurrentLevelHeight() const;

private:
	std::array<bool, 349> m_keys;
	std::array<bool, 15> m_mouseButtons;
	glm::dvec2 m_cursorPos;
	glm::dvec2 m_scrollOffset;

	glm::dvec2 m_firstPos;

	enum class EGameState {
		Active,
		Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Tank> m_pTank;

	std::unique_ptr<Level> m_pLevel;

	std::shared_ptr<RenderEngine::Model3D> m_pModel;
	std::shared_ptr<RenderEngine::Light> m_pLight;

	double m_time;

	glm::vec3 m_cameraPosition;
	glm::vec3 m_cameraRotation;
	glm::vec3 m_cameraScale;

	void keyUp(const int action);
	void keyDown(const int action);
	void keyLeft(const int action);
	void keyRight(const int action);
	void keyLeftShift(const int action);
	void keySpace(const int action);
	void keyW(const int action);
	void keyA(const int action);
	void keyS(const int action);
	void keyD(const int action);
	void keyQ(const int action);
	void keyE(const int action);
	void keyMouseLeft(const int action);
};