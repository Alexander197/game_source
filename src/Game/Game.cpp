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

Game::Game(const glm::ivec2& windowSize) :m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize),
            m_cameraPosition(glm::vec3(0.0f, 0.0f, -550.0f)), m_cameraRotation(glm::vec3(0.0f, 0.0f, 0.0f)),
            m_cameraScale(glm::vec3(1.0f, 1.0f, 1.0f))
{
	m_keys.fill(false);
    m_mouseButtons.fill(false);
    m_cursorPos = glm::dvec2(0.0, 0.0);
    m_firstPos = glm::dvec2(0.0, 0.0);
    m_scrollOffset = glm::dvec2(0.0, 0.0);
}
Game::~Game()
{

}

void Game::render()
{
    //ResourceManager::getAnimatedSprite("anim1")->render();
    //ResourceManager::getSprite("Tex1Sprite")->render();
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, m_cameraPosition);

    float angleX = glm::radians(m_cameraRotation.x);
    float angleY = glm::radians(m_cameraRotation.y);
    float angleZ = glm::radians(m_cameraRotation.z);
    viewMatrix = glm::rotate(viewMatrix, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, angleZ, glm::vec3(0.0f, 0.0f, 1.0f));
    
    viewMatrix = glm::scale(viewMatrix, m_cameraScale);

    pSpriteShaderProgram->setMatrix4("viewMat", viewMatrix);


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
        for (size_t i = 0; i < size; i++)
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
        if (m_keys[GLFW_KEY_W])
        {
            m_cameraPosition += glm::vec3(0.0f, -1.0f, 0.0f);
            //m_cameraRotation += glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else if (m_keys[GLFW_KEY_A])
        {
            m_cameraPosition += glm::vec3(1.0f, 0.0f, 0.0f);
            //m_cameraRotation += glm::vec3(0.0f, -1.0f, 0.0f);
        }

        if (m_keys[GLFW_KEY_S])
        {
            m_cameraPosition += glm::vec3(0.0f, 1.0f, 0.0f);
            //m_cameraRotation += glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        else if (m_keys[GLFW_KEY_D])
        {
            m_cameraPosition += glm::vec3(-1.0f, 0.0f, 0.0f);
            //m_cameraRotation += glm::vec3(0.0f, 1.0f, 0.0f);
        }

        if (m_keys[GLFW_KEY_Q])
        {
            //m_cameraPosition += glm::vec2(0.0f, -1.0f);
            m_cameraRotation += glm::vec3(0.0f, 0.0f, 0.5f);
        }
        else if (m_keys[GLFW_KEY_E])
        {
            //m_cameraPosition += glm::vec2(1.0f, 0.0f);
            m_cameraRotation += glm::vec3(0.0f, 0.0f, -0.5f);
        }

        if (m_mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
        {
            glm::vec2 cursorPosSubtract = m_firstPos - m_cursorPos;
            m_cameraRotation = glm::vec3( cursorPosSubtract.y, cursorPosSubtract.x, 0.0f);
        }

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
        break;
    case GLFW_KEY_SPACE:
        keySpace(action);
        break;
    case GLFW_KEY_W:
        keyW(action);
        break;
    case GLFW_KEY_A:
        keyA(action);
        break;
    case GLFW_KEY_S:
        keyS(action);
        break;
    case GLFW_KEY_D:
        keyD(action);
        break;
    case GLFW_KEY_Q:
        keyQ(action);
        break;
    case GLFW_KEY_E:
        keyE(action);
        break;
    
    }
}
void Game::setMouseButton(const int button, const int action)
{
    m_mouseButtons[button] = action;
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        keyMouseLeft(action);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        break;
    }
}
void Game::setCursorPos(const double xpos, const double ypos)
{
    m_cursorPos = glm::dvec2(xpos, ypos);
}
void Game::setScrollOffset(const double xoffset, const double yoffset)
{
    m_scrollOffset = glm::dvec2(xoffset, yoffset);
    //m_cameraScale += glm::vec3(yoffset / 10.f, yoffset / 10.f, 0.0f);
    m_cameraPosition += glm::vec3(0.0f, 0.0f, static_cast<float>(yoffset) * 15.0f);
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

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[1]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    //m_cameraPosition = glm::vec3(static_cast<float>(m_windowSize.x) / 2.0f, static_cast<float>(m_windowSize.y) / 2.0f, 0.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0);

    //glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -1000.0f, 1000.0f);
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y), 0.1f, 2000.0f);
   
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
    

    m_pTank = std::make_unique<Tank>(ResourceManager::getSprite("yellowTank_1"),
        glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.003f, 0.003f, 0.0f }, 0.1f, m_pLevel->getPlayerRespawn_1(), 
        glm::vec2(Level::BLOCK_SIZE / 1.0, Level::BLOCK_SIZE / 1.0), 1.0f);

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
            //m_pTank->setVelocity(0.05);
            break;
        case GLFW_RELEASE:
            //m_pTank->setVelocity();
            break;
    }
}
void Game::keySpace(const int action)
{
}
void Game::keyW(const int action)
{
    //if(action)
        //m_cameraPosition += glm::vec2(0.0f, 1.0f);
}
void Game::keyA(const int action)
{
    //if (action)
        //m_cameraPosition += glm::vec2(-1.0f, 0.0f);
}
void Game::keyS(const int action)
{
    //if (action)
        //m_cameraPosition += glm::vec2(0.0f, -1.0f);
}
void Game::keyD(const int action)
{
    //if (action)
        //m_cameraPosition += glm::vec2(1.0f, 0.0f);
}
void Game::keyQ(const int action)
{
    //if (action)
        //m_cameraPosition += glm::vec2(0.0f, -1.0f);
}
void Game::keyE(const int action)
{
    //if (action)
        //m_cameraPosition += glm::vec2(1.0f, 0.0f);
}
void Game::keyMouseLeft(const int action)
{
    switch (action)
    {
    case GLFW_PRESS:
       m_firstPos = m_cursorPos;
        break;
    case GLFW_RELEASE:
        m_firstPos = m_cursorPos;
        break;
    }
}
