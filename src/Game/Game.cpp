#include "Tank.h"
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
}
void Game::update(const uint64_t delta)
{
    //ResourceManager::getAnimatedSprite("anim1")->update(delta);

    if (m_pTank) {
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
   
}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}
bool Game::init()
{
    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");

    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create sprite shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    /*std::vector<std::string> subTextureNames = {
        "TankYellowUp1",
        "TankYellowUp2",
        "TankYellowLeft1",
        "TankYellowLeft2",
        "TankYellowDown1",
        "TankYellowDown2",
        "TankYellowRight1",
        "TankYellowRight2" };

    auto textureAtlas = ResourceManager::loadTextureAtlas("DafaultTextureAtlas", "res/textures/tank_yellow.png", subTextureNames, 32, 32);

    auto pTex1Sprite = ResourceManager::loadSprite("Tex1Sprite", "DafaultTextureAtlas", "SpriteShader", 32, 32, "TankYellowUp1");
    pTex1Sprite->setPosition(glm::vec2(300, 300));

    auto pAnim1 = ResourceManager::loadAnimatedSprite("anim1", "DafaultTextureAtlas", "SpriteShader", 32, 32, "TankYellowUp1");

    std::vector<std::pair<std::string, uint64_t>> someState1;
    someState1.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowLeft1", 200000000));
    someState1.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowLeft2", 200000000));

    pAnim1->insertState("someState1", someState1);

    pAnim1->setPosition(glm::vec2(100, 100));
    pAnim1->setState("someState1");*/

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);



    std::vector<std::string> tanksSubTextureNames = {
        "TankYellowUp1",
        "TankYellowUp2",
        "TankYellowLeft1",
        "TankYellowLeft2",
        "TankYellowDown1",
        "TankYellowDown2",
        "TankYellowRight1",
        "TankYellowRight2" };

    auto tanksTextureAtlas = ResourceManager::loadTextureAtlas("TanksTextureAtlas", "res/textures/tanks.png", tanksSubTextureNames, 32, 32);
    auto pYTank = ResourceManager::loadAnimatedSprite("YTankSprite", "TanksTextureAtlas", "SpriteShader", 32, 32, "TankYellowUp1");

    std::vector<std::pair<std::string, uint64_t>> YTankUpState;
    YTankUpState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowUp1", 200000000));
    YTankUpState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowUp2", 200000000));

    std::vector<std::pair<std::string, uint64_t>> YTankLeftState;
    YTankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowLeft1", 200000000));
    YTankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowLeft2", 200000000));

    std::vector<std::pair<std::string, uint64_t>> YTankDownState;
    YTankDownState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowDown1", 200000000));
    YTankDownState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowDown2", 200000000));

    std::vector<std::pair<std::string, uint64_t>> YTankRightState;
    YTankRightState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowRight1", 200000000));
    YTankRightState.emplace_back(std::make_pair<std::string, uint64_t>("TankYellowRight2", 200000000));

    pYTank->insertState("tankUpState", std::move(YTankUpState));
    pYTank->insertState("tankLeftState", std::move(YTankLeftState));
    pYTank->insertState("tankDownState", std::move(YTankDownState));
    pYTank->insertState("tankRightState", std::move(YTankRightState));

    pYTank->setState("tankUpState");
    pYTank->setPosition(glm::vec2(200.0f, 200.0f));

    m_pTank = std::make_unique<Tank>(pYTank, 0.0000001f, glm::vec2(200.0f, 200.0f));

    return true;
}