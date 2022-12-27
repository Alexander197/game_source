# pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>

#include "../Renderer/Model3D.h"
#include "../Renderer/LightSource.h"
#include "../Renderer/Text.h"
#include "../Renderer/SkyBox.h"
#include "../Renderer/PostProcess.h"

#include "../Renderer/generation/plainMesh.h"

#include "Camera/Camera.h"

#include <iostream>

#include "../Renderer/Light.h"

class Tank;
class Level;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void depthRender();
	void update(const double delta);
	bool init();

	void setWindowSize(const glm::ivec2 windowSize);
	
	void setKey(const int key, const int action);
	void setMouseButton(const int button, const int action);
	void setCursorPos(const double xpos, const double ypos);
	void setScrollOffset(const double xoffset, const double yoffset);

	void setFPS(float fps) { m_fps = fps; }
	

	size_t getCurrentLevelWidth() const;
	size_t getCurrentLevelHeight() const;

private:
	std::array<bool, 349> m_keys;
	std::array<bool, 15> m_mouseButtons;

	glm::dvec2 m_currentCursorPos;
	glm::dvec2 m_lastCursorPos;

	enum class EGameState {
		Active,
		Pause
	};

	glm::ivec2 m_windowSize;
	EGameState m_eCurrentGameState;
	std::unique_ptr<Tank> m_pTank;

	std::unique_ptr<Level> m_pLevel;

	std::shared_ptr<RenderEngine::Model3D> m_pModel;
	std::shared_ptr<RenderEngine::Model3D> m_pModel1;

	std::shared_ptr<RenderEngine::LightSource> m_pLightSource;

	std::shared_ptr<RenderEngine::DirLigth> m_pDirLight0;
	std::shared_ptr<RenderEngine::DirLigth> m_pDirLight1;

	std::shared_ptr<RenderEngine::Light> m_pLight;

	std::unique_ptr<RenderEngine::SkyBox> m_pSkyBox;

	std::unique_ptr<RenderEngine::PlainMesh> m_pPlainMesh;

	std::unique_ptr<RenderEngine::Text> m_pLog;

	double m_time;
	float m_fps;

	std::unique_ptr<Camera> m_pCamera;

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
	void keyP(const int action);
	void keyF(const int action);

	void keyMouseLeft(const int action);
	void keyMouseRight(const int action);

	float m_isFlashLightOn;
	int m_blinn;

	float coff = 16;
	std::unique_ptr<RenderEngine::PostProcess> m_pPostProcess;
};