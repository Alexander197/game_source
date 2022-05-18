#include "GameObjects/Tank.h"
#include "Level.h"
#include "Game.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

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
   
    
    if (m_pLevel) {
        m_pLevel->render();
    }
    if (m_pTank) {
        m_pTank->render();
    }
    
}

void Game::update(const double delta)
{
    if (m_pLevel) {
        m_pLevel->update(delta);
    }

    if (m_pTank) 
    {
        std::vector<std::shared_ptr<IGameObject>> mapObjects = m_pLevel->getMapObjects();
        size_t size = mapObjects.size();
        for (size_t i = 0; i < size - 10; i++)
        {
            if(mapObjects[i])
            if (BoundingBox::isColide(m_pTank->getBoundingBox(), mapObjects[i]->getBoundingBox()))
            {
                m_pTank->setLastPosition();
                m_pTank->move(false);
                break;
            }
        }
        /*if (m_keys[GLFW_KEY_UP])
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
        }*/

        m_pTank->update(delta);
    }
   
}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
    switch (key)
    {
    case GLFW_KEY_UP:
        keyUp(action);
        break;
    case GLFW_KEY_LEFT:
        keyLeft(action);
        break;
    case GLFW_KEY_DOWN:
        keyDown(action);
        break;
    case GLFW_KEY_RIGHT:
        keyRight(action);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        keyLeftShift(action);
    }
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

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[0]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    
    m_pTank = std::make_unique<Tank>(ResourceManager::getSprite("yellowTank_1"),
        0.0f, 0.003f, 0.1f, m_pLevel->getPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE / 1.3, Level::BLOCK_SIZE / 1.3), 1.0f);

    return true;
}

size_t Game::getCurrentLevelWidth() const
{
    return m_pLevel->getLevelWidth();
}
size_t Game::getCurrentLevelHeight() const
{
    return m_pLevel->getLevelHeight();
}

void Game::keyUp(const int action)
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
    if (action)
    {
        m_pTank->setOrientation(Tank::EOrientation::Up);
        m_pTank->move(true);
    }
}
void Game::keyDown(const int action)
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
    if (action)
    {
        m_pTank->setOrientation(Tank::EOrientation::Down);
        m_pTank->move(true);
    }
}
void Game::keyLeft(const int action)
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
    if (action)
    {
        m_pTank->setOrientation(Tank::EOrientation::Left);
        m_pTank->move(true);
    }
}
void Game::keyRight(const int action)
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
    if (action)
    {
        m_pTank->setOrientation(Tank::EOrientation::Right);
        m_pTank->move(true);
    }
}
void Game::keyLeftShift(const int action)
{
    switch (action)
    {
        case GLFW_PRESS:
            m_pTank->setVelocity(0.05);
            break;
        case GLFW_RELEASE:
            m_pTank->setVelocity(0.1);
            break;
    }
}