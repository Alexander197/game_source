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

Game::Game(const glm::ivec2& windowSize) :m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
    m_mouseButtons.fill(false);
    m_currentCursorPos = glm::dvec2(0.0, 0.0);
    m_lastCursorPos = glm::dvec2(0.0, 0.0);
    m_time = 0.0;
    m_fps = 0.0;

    m_isFlashLightOn = -1;
}
Game::~Game()
{

}
void Game::render()
{
    //ResourceManager::getAnimatedSprite("anim1")->render();
    //ResourceManager::getSprite("Tex1Sprite")->render();
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    auto p3DModelShaderProgram = ResourceManager::getShaderProgram("3DModelShader");
    auto pLightShaderProgram = ResourceManager::getShaderProgram("lightShader");
    auto pTextShaderProgram = ResourceManager::getShaderProgram("textShader");

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_pCamera->getFov()), static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y), 0.1f, 2000.0f);

    glm::mat4 viewMatrix = m_pCamera->getLookAtMatrix();

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setMat4("viewMat", viewMatrix);
    pSpriteShaderProgram->setMat4("projectionMat", projectionMatrix);

    p3DModelShaderProgram->use();
    //p3DModelShaderProgram->setVec3("dirLight[0].position", m_pLightSource->getPosition());
    
    //p3DModelShaderProgram->setVec3("light.direction", m_isFlashLightOn * m_pCamera->getFront());
    //p3DModelShaderProgram->setFloat("light.cutOff", glm::cos(glm::radians(5.0f)));
    //p3DModelShaderProgram->setFloat("light.outerCutOff", glm::cos(glm::radians(30.0f)));

    //p3DModelShaderProgram->setVec3("dirLight[0].direction", glm::vec3(20.0f, 20.0f, 20.0f));
    //p3DModelShaderProgram->setVec3("dirLight[1].direction", glm::vec3(-20.0f, -20.0f, -20.0f));

    p3DModelShaderProgram->setVec3("pointLight[0].position", m_pLightSource->getPosition());

    p3DModelShaderProgram->setVec3("spotLight[0].position", m_isFlashLightOn * m_pCamera->getPosition());
    p3DModelShaderProgram->setVec3("spotLight[0].direction", m_pCamera->getFront());

    p3DModelShaderProgram->setFloat("spotLight[0].cutOff", glm::cos(glm::radians(5.0)));
    p3DModelShaderProgram->setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0)));

    p3DModelShaderProgram->setVec3("viewPos", m_pCamera->getPosition());
    p3DModelShaderProgram->setMat4("viewMat", viewMatrix);
    
    p3DModelShaderProgram->setMat4("projectionMat", projectionMatrix);

    pLightShaderProgram->use();
    pLightShaderProgram->setMat4("viewMat", viewMatrix);
    pLightShaderProgram->setMat4("projectionMat", projectionMatrix);

    pTextShaderProgram->use();

    if (m_pLevel) {
       //m_pLevel->render();
    }
    if (m_pTank) {
       //m_pTank->render();
    }
    if (m_pModel)
    {
        m_pModel->render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        //m_pModel->render(glm::vec3(-9.0f, 8.0f, 10.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(11.0f, 59.0f, 12.0f));
        //m_pModel->render(glm::vec3(12.0f, -10.0f, 14.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(60.0f, 30.0f, 0.0f));
        //m_pModel->render(glm::vec3(15.0f, 8.0f, 8.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(43.0f, 55.0f, 53.0f));
        //m_pModel->render(glm::vec3(11.0f, 5.0f, -7.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(23.0f, 12.0f, 80.0f));
        //m_pModel->render(glm::vec3(-7.0f, 9.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(65.0f, 23.0f, 43.0f));
        //m_pModel->render(glm::vec3(15.0f, -12.0f, 10.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(78.0f, 63.0f, 12.0f));
        //m_pModel->render(glm::vec3(12.0f, 9.0f, -12.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(15.0f, 4.0f, 56.0f));
    }
    if (m_pLightSource)
    {
        m_pLightSource->render();
    }
    if (m_pFps)
    {
        std::string fps = std::to_string(static_cast<int>(m_fps));
        m_pFps->render("FPS " + fps, glm::vec2(0.0, m_windowSize.y - 30.0f), glm::vec2(30.0f,30.0f), 10.0f);
    }
}

void Game::update(const double delta)
{
    if (m_eCurrentGameState == EGameState::Active)
    {
        if (m_pLevel) {
            m_pLevel->update(delta);
        }

        m_time += delta;

        if (m_pLightSource)
        {
            //m_pLightSource->setPosition(glm::vec3(50.0 * sin(m_time / 800.0), 0.0, 50.0 * sin(m_time / 3200.0)));
        }
        if (m_pTank)
        {
            std::vector<std::shared_ptr<IGameObject>> mapObjects = m_pLevel->getMapObjects();
            size_t size = mapObjects.size();
            for (size_t i = 0; i < size; i++)
            {
                if (mapObjects[i])
                    if (AxisAlignedBoundingBox::isCollide(m_pTank->getBoundingBox(), mapObjects[i]->getBoundingBox()) == 1)
                    {
                        m_pTank->setLastPosition();
                        m_pTank->move(false);
                        break;
                    }
            }
            m_pTank->update(delta);
        }
        if (m_pCamera)
        {
            if (m_keys[GLFW_KEY_W])
            {
                m_pCamera->moveForward(delta);
            }
            else if (m_keys[GLFW_KEY_A])
            {
                m_pCamera->moveLeft(delta);
            }

            if (m_keys[GLFW_KEY_S])
            {
                m_pCamera->moveBackward(delta);
            }
            else if (m_keys[GLFW_KEY_D])
            {
                m_pCamera->moveRight(delta);
            }

            if (m_keys[GLFW_KEY_SPACE])
            {
                m_pCamera->moveUp(delta);
            }
            else if (m_keys[GLFW_KEY_LEFT_SHIFT])
            {
                m_pCamera->moveDown(delta);
            }

            if (m_keys[GLFW_KEY_E])
            {
                m_pCamera->setRoll(1.0f, delta);
            }
            else if (m_keys[GLFW_KEY_Q])
            {
                m_pCamera->setRoll(-1.0f, delta);
            }

            if (true)
            {
                glm::vec2 cursorPosSubtract = m_lastCursorPos - m_currentCursorPos;
                m_pCamera->setPitch(cursorPosSubtract.y, delta);
                m_pCamera->setYaw(-cursorPosSubtract.x, delta);
                m_lastCursorPos = m_currentCursorPos;
            }
        }
    }
}

bool Game::init()
{
    ResourceManager::loadJSONResources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    auto p3DModelShaderProgram = ResourceManager::getShaderProgram("3DModelShader");
    auto pLightShaderProgram = ResourceManager::getShaderProgram("lightShader");
    auto pTextShaderProgram = ResourceManager::getShaderProgram("textShader");

    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }
    if (!p3DModelShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "3DModelShader" << std::endl;
        return false;
    }
    if (!pLightShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "lightShader" << std::endl;
        return false;
    }

    m_pLevel = std::make_unique<Level>(ResourceManager::getLevels()[1]);
    m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    glm::mat4 projectionMatrixO = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);

    glm::mat4 projectionMatrixP = glm::perspective(glm::radians(60.0f), static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y), 0.1f, 2000.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMat4("projectionMat", projectionMatrixP);


    pLightShaderProgram->use();
    pLightShaderProgram->setMat4("projectionMat", projectionMatrixP);

    m_pLightSource = std::make_shared<RenderEngine::LightSource>(pLightShaderProgram, glm::vec3(20.0f, 20.0f, 20.0f));

    p3DModelShaderProgram->use();

    p3DModelShaderProgram->setMat4("projectionMat", projectionMatrixP);

    m_pTank = std::make_unique<Tank>(ResourceManager::getSprite("yellowTank_1"),
        glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.003f, 0.003f, 0.0f }, 0.1f, m_pLevel->getPlayerRespawn_1(),
        glm::vec2(Level::BLOCK_SIZE / 1.0, Level::BLOCK_SIZE / 1.0), 0.0f);

    m_pModel = ResourceManager::get3DModel("cube");
    m_pCamera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));

    pTextShaderProgram->use();
    pTextShaderProgram->setMat4("projectionMat", projectionMatrixO);

    m_pFps = std::make_unique<RenderEngine::Text>(ResourceManager::getTexture("textArial256"), ResourceManager::getShaderProgram("textShader"));

    return true;
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
    case GLFW_KEY_P:
        keyP(action);
        break;
    case GLFW_KEY_F:
        keyF(action);
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
    m_currentCursorPos = glm::dvec2(xpos, ypos);
}
void Game::setScrollOffset(const double xoffset, const double yoffset)
{
    m_pCamera->setFov(-static_cast<float>(yoffset), 1.0f);
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
void Game::keyP(const int action)
{
    if (action == GLFW_PRESS)
    {
        if (m_eCurrentGameState == EGameState::Active)
        {
            m_eCurrentGameState = EGameState::Pause;
        }
        else
        {
            m_eCurrentGameState = EGameState::Active;
        }
    }
}
void Game::keyF(const int action)
{
    if (action == GLFW_PRESS)
    {
        m_isFlashLightOn *= -1;
    }
}
void Game::keyMouseLeft(const int action)
{
    switch (action)
    {
    case GLFW_PRESS:
       m_lastCursorPos = m_currentCursorPos;
        break;
    case GLFW_RELEASE:
        break;
    }
}
