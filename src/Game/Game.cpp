#include "Game.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include <iostream>

Game::Game(const glm::ivec2& windowSize):m_eCurrentGameState(EGameState::Active), m_windowSize(windowSize)
{
	m_keys.fill(false);
}
Game::~Game()
{

}

void Game::render()
{
    ResourceManager::getAnimatedSprite("anim1")->render();
    ResourceManager::getSprite("Tex1Sprite")->render();

}
void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("anim1")->update(delta);
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

    std::vector<std::string> subTextureNames = {
        "tex1",
        "tex2",
        "tex3",
        "tex4",
        "tex5",
        "tex6",
        "tex7",
        "tex8" };

    auto textureAtlas = ResourceManager::loadTextureAtlas("DafaultTextureAtlas", "res/textures/different_textures.jpg", subTextureNames, 100, 100);

    auto pTex1Sprite = ResourceManager::loadSprite("Tex1Sprite", "DafaultTextureAtlas", "SpriteShader", 100, 100, "tex2");
    pTex1Sprite->setPosition(glm::vec2(300, 300));

    auto pAnim1 = ResourceManager::loadAnimatedSprite("anim1", "DafaultTextureAtlas", "SpriteShader", 100, 100, "tex1");

    std::vector<std::pair<std::string, uint64_t>> someState1;
    someState1.emplace_back(std::make_pair<std::string, uint64_t>("tex1", 1000000000));
    someState1.emplace_back(std::make_pair<std::string, uint64_t>("tex2", 1000000000));
    someState1.emplace_back(std::make_pair<std::string, uint64_t>("tex3", 1000000000));

    pAnim1->insertState("someState1", someState1);

    pAnim1->setPosition(glm::vec2(100, 100));
    pAnim1->setState("someState1");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    return true;
}