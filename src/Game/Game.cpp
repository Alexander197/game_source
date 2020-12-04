#include "GameObjects/Tank.h"
#include "Level.h"
#include "Game.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <iostream>
#include <GLFW\glfw3.h>
#include <thread>

Game::Game(const glm::ivec2& windowSize):m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
}
Game::~Game()
{

}

void Game::render()
{
    //ResourceManager::getAnimatedSprite("anim1")->render();
    //ResourceManager::getSprite("Tex1Sprite")->render();
   
    if (m_pTank) {
        m_pTank->render();
    }
    if (m_waterSprite) {
        m_waterSprite->render(glm::vec2(300.0f, 300.0f), glm::vec2(32.0f, 32.0f), 0.0f);
    }
    if (m_pLevel) {
        m_pLevel->render();
    }
}

void Game::update(const uint64_t delta)
{
    if (m_pLevel) {
        m_pLevel->update(delta);
    }

    if (m_pTank) 
    {
        if (m_keys[GLFW_KEY_UP])
        {
            m_pTank->setOrientation(Tank::EOrientation::Up);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_LEFT])
        {
            m_pTank->setOrientation(Tank::EOrientation::Left);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_DOWN])
        {
            m_pTank->setOrientation(Tank::EOrientation::Down);
            m_pTank->move(true);
        }
        else if (m_keys[GLFW_KEY_RIGHT])
        {
            m_pTank->setOrientation(Tank::EOrientation::Right);
            m_pTank->move(true);
        }
        else 
        {
            m_pTank->move(false);
        }

        m_pTank->update(delta);
    }
    if (m_waterSprite) {
        m_waterSprite->update(delta);
    }
   
}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}
bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    auto pYellowTank_1 = ResourceManager::getAnimatedSprite("yellowTank_1");
    m_waterSprite = ResourceManager::getAnimatedSprite("waterSprite");

    if (!pYellowTank_1)
    {
        std::cerr << "Can't find animated sprite: " << "yellowTank_1" << std::endl;
        return false;
    }
    if (!m_waterSprite)
    {
        std::cerr << "Can't find animated sprite: " << "waterSprite" << std::endl;
        return false;
    }

    pYellowTank_1->setState("tankUpState");

    m_waterSprite->setState("waterFlow");

    m_pTank = std::make_unique<Tank>(pYellowTank_1, 0.0000001f, glm::vec2(200.0f, 200.0f), glm::vec2(32.0f, 32.0f));
    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);

    return true;
}