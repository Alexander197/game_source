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
    m_pDirLight0(std::make_shared<RenderEngine::DirLigth>(glm::vec3(-10.0f, 10.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.1f, 0.1f, 0.1f))),
    m_pDirLight1(std::make_shared<RenderEngine::DirLigth>(glm::vec3(10.0f, 10.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.1f, 0.1f, 0.1f)))
{
	m_keys.fill(false);
    m_mouseButtons.fill(false);
    m_currentCursorPos = glm::dvec2(0.0, 0.0);
    m_lastCursorPos = glm::dvec2(0.0, 0.0);
    m_time = 0.0;
    m_fps = 0.0;

    m_isFlashLightOn = -1;
    m_blinn = 1;
}
Game::~Game()
{

}
void Game::depthRender()
{
    auto pSimpleDepthShader = ResourceManager::getShaderProgram("simpleDepthShader");
    m_pLight->depthRender();
    pSimpleDepthShader->use();
    pSimpleDepthShader->setMat4("lightSpaceMatrix", m_pLight->lightSpaceMatrix);
    if (m_pModel && m_pModel1)
    {
        m_pModel->depthRender(glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
        m_pModel->depthRender(glm::vec3(1.8f, 0.0f, 1.8f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
        m_pModel1->depthRender(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(60.0f, 6.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_windowSize.x, m_windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_pLight->depthRender1();
    pSimpleDepthShader->use();
    pSimpleDepthShader->setMat4("lightSpaceMatrix", m_pLight->lightSpaceMatrix1);
    if (m_pModel && m_pModel1)
    {
        m_pModel->depthRender(glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
        m_pModel->depthRender(glm::vec3(1.8f, 0.0f, 1.8f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
        m_pModel1->depthRender(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(60.0f, 6.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f), pSimpleDepthShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_windowSize.x, m_windowSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void Game::render()
{
    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    auto p3DModelShaderProgram = ResourceManager::getShaderProgram("3DModelShader");
    auto pLightShaderProgram = ResourceManager::getShaderProgram("lightShader");
    auto pTextShaderProgram = ResourceManager::getShaderProgram("textShader");
    auto pStencilShader = ResourceManager::getShaderProgram("stencilShader");
    auto pSkyBoxShaderProgram = ResourceManager::getShaderProgram("skyBoxShader");
    auto pPostProcessShaderProgram = ResourceManager::getShaderProgram("postProcessShader");
    auto pPlainMeshShaderProgram = ResourceManager::getShaderProgram("plainMeshShader");

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_pCamera->getFov()), 
        static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y), 0.1f, 2000.0f);

    glm::mat4 viewMatrix = m_pCamera->getLookAtMatrix();

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setMat4("viewMat", viewMatrix);
    pSpriteShaderProgram->setMat4("projectionMat", projectionMatrix);

    p3DModelShaderProgram->use();

    auto dirLigths = m_pLight->getDirLights();
    for (size_t i = 0; i < dirLigths.size(); i++)
    {
        p3DModelShaderProgram->setVec3(("dirLight[" + std::to_string(i) + "].direction").c_str(), dirLigths[i]->direction);

        p3DModelShaderProgram->setVec3(("dirLight[" + std::to_string(i) + "].ambient").c_str(), dirLigths[i]->ambient);
        p3DModelShaderProgram->setVec3(("dirLight[" + std::to_string(i) + "].diffuse").c_str(), dirLigths[i]->diffuse);
        p3DModelShaderProgram->setVec3(("dirLight[" + std::to_string(i) + "].specular").c_str(), dirLigths[i]->specular);
    }
    auto pointLights = m_pLight->getPointLighst();
    for (size_t i = 0; i < pointLights.size(); i++)
    {
        p3DModelShaderProgram->setVec3(("pointLight[" + std::to_string(i) + "].position").c_str(), pointLights[i]->position);

        p3DModelShaderProgram->setVec3(("pointLight[" + std::to_string(i) + "].ambient").c_str(), pointLights[i]->ambient);
        p3DModelShaderProgram->setVec3(("pointLight[" + std::to_string(i) + "].diffuse").c_str(), pointLights[i]->diffuse);
        p3DModelShaderProgram->setVec3(("pointLight[" + std::to_string(i) + "].specular").c_str(), pointLights[i]->specular);

        p3DModelShaderProgram->setFloat(("pointLight[" + std::to_string(i) + "].constant").c_str(), pointLights[i]->constant);
        p3DModelShaderProgram->setFloat(("pointLight[" + std::to_string(i) + "].linear").c_str(), pointLights[i]->linear);
        p3DModelShaderProgram->setFloat(("pointLight[" + std::to_string(i) + "].quadratic").c_str(), pointLights[i]->quadratic);

    }
    auto spotLights = m_pLight->getSpotLights();
    for (size_t i = 0; i < spotLights.size(); i++)
    {
        p3DModelShaderProgram->setVec3(("spotLight[" + std::to_string(i) + "].position").c_str(), spotLights[i]->position);
        p3DModelShaderProgram->setVec3(("spotLight[" + std::to_string(i) + "].direction").c_str(), spotLights[i]->direction);

        p3DModelShaderProgram->setVec3(("spotLight[" + std::to_string(i) + "].ambient").c_str(), spotLights[i]->ambient);
        p3DModelShaderProgram->setVec3(("spotLight[" + std::to_string(i) + "].diffuse").c_str(), spotLights[i]->diffuse);
        p3DModelShaderProgram->setVec3(("spotLight[" + std::to_string(i) + "].specular").c_str(), spotLights[i]->specular);

        p3DModelShaderProgram->setFloat(("spotLight[" + std::to_string(i) + "].constant").c_str(), spotLights[i]->constant);
        p3DModelShaderProgram->setFloat(("spotLight[" + std::to_string(i) + "].linear").c_str(), spotLights[i]->linear);
        p3DModelShaderProgram->setFloat(("spotLight[" + std::to_string(i) + "].quadratic").c_str(), spotLights[i]->quadratic);

        p3DModelShaderProgram->setFloat(("spotLight[" + std::to_string(i) + "].cutOff").c_str(), spotLights[i]->cutOff);
        p3DModelShaderProgram->setFloat(("spotLight[" + std::to_string(i) + "].outerCutOff").c_str(), spotLights[i]->outerCutOff);
    }
    //p3DModelShaderProgram->setVec3("spotLight[0].position", m_isFlashLightOn * m_pCamera->getPosition());
    //p3DModelShaderProgram->setVec3("spotLight[0].direction", m_pCamera->getFront());
    //p3DModelShaderProgram->setFloat("spotLight[0].cutOff", glm::cos(glm::radians(5.0)));
    //p3DModelShaderProgram->setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(20.0)));

    p3DModelShaderProgram->setVec3("viewPos", m_pCamera->getPosition());
    p3DModelShaderProgram->setMat4("viewMat", viewMatrix);
    
    p3DModelShaderProgram->setMat4("projectionMat", projectionMatrix);

    p3DModelShaderProgram->setInt("blinn", m_blinn);

    pLightShaderProgram->use();
    pLightShaderProgram->setMat4("viewMat", viewMatrix);
    pLightShaderProgram->setMat4("projectionMat", projectionMatrix);

    pStencilShader->use();
    pStencilShader->setMat4("viewMat", viewMatrix);
    pStencilShader->setMat4("projectionMat", projectionMatrix);

    pSkyBoxShaderProgram->use();
    pSkyBoxShaderProgram->setMat4("viewMat", glm::mat4(glm::mat3(viewMatrix)));
    //pSkyBoxShaderProgram->setMat4("viewMat", viewMatrix);
    pSkyBoxShaderProgram->setMat4("projectionMat", projectionMatrix);

    pPlainMeshShaderProgram->use();
    pPlainMeshShaderProgram->setMat4("viewMat", viewMatrix);
    pPlainMeshShaderProgram->setMat4("projectionMat", projectionMatrix);

    //pTextShaderProgram->use();

    /*if (m_pPostProcess)
    {
        m_pPostProcess->capture();
    }*/
        if (m_pLevel) {
           //m_pLevel->render();
        }
        if (m_pTank) {
           //m_pTank->render();
        }
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);
        depthRender();
        //glCullFace(GL_BACK);

        p3DModelShaderProgram->use();
        p3DModelShaderProgram->setMat4("lightSpaceMatrix", m_pLight->lightSpaceMatrix);
        p3DModelShaderProgram->setMat4("lightSpaceMatrix1", m_pLight->lightSpaceMatrix1);
        if (m_pModel)
        {   
            //glEnable(GL_CULL_FACE);
            m_pModel->render(glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f));
            m_pModel->render(glm::vec3(1.8f, 0.0f, 1.8f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.0f, 0.0f, 0.0f));
            m_pModel1->render(glm::vec3(0.0f, -6.0f, 0.0f), glm::vec3(60.0f, 6.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        }
        if (m_pLightSource)
        {
            m_pLightSource->setPosition(m_pDirLight0->direction);
            m_pLightSource->render();
            m_pLightSource->setPosition(m_pDirLight1->direction);
            m_pLightSource->render();
        }
        if (m_pPlainMesh)
        {
            //m_pPlainMesh->render(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f));
        }
        if (m_pSkyBox)
        {
            m_pSkyBox->render();
        }
    /*if (m_pPostProcess)
    {
        pPostProcessShaderProgram->use();
        pPostProcessShaderProgram->setFloat("offset", 1 / 300.0f);

        pPostProcessShaderProgram->setFloat("kernel[0]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[1]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[2]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[3]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[4]",  10.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[5]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[6]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[7]",  -1.0f / coff);
        pPostProcessShaderProgram->setFloat("kernel[8]",  -1.0f / coff);
       
        
        m_pPostProcess->render();
    }*/
    if (m_pLog)
    {
        std::string fps = std::to_string(static_cast<int>(m_fps));
        float size = 30.0f;
        m_pLog->render("FPS " + fps, glm::vec2(0.0, m_windowSize.y - size), glm::vec2(size, size), 10.0f);

        if (m_blinn)
        {
            m_pLog->render("SPEC PHONG", glm::vec2(0.0, m_windowSize.y - 2 * size), glm::vec2(size, size), 10.0f);
        }
        else
        {
            m_pLog->render("SPEC BLINN", glm::vec2(0.0, m_windowSize.y - 2 * size), glm::vec2(size, size), 10.0f);
        }
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

        if (m_pDirLight0)
        {
            m_pDirLight0->direction = glm::vec3(10.0 * sin(m_time / 800.0), 10.0, 10.0 * sin(m_time / 800.0));
        }
        if (m_pDirLight1)
        {
            //m_pDirLight1->direction = glm::vec3(10.0 * cos(m_time / 2400.0), 10.0, 10.0 * sin(m_time / 800.0));
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
    auto pSkyBoxShaderProgram = ResourceManager::getShaderProgram("skyBoxShader");
    auto pPostProcessShaderProgram = ResourceManager::getShaderProgram("postProcessShader");
    auto pPlainMeshShaderProgram = ResourceManager::getShaderProgram("plainMeshShader");

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
    //m_windowSize.x = static_cast<int>(m_pLevel->getLevelWidth());
    //m_windowSize.y = static_cast<int>(m_pLevel->getLevelHeight());

    glm::mat4 projectionMatrixO = glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);

    glm::mat4 projectionMatrixP = glm::perspective(glm::radians(60.0f), static_cast<float>(m_windowSize.x) / static_cast<float>(m_windowSize.y), 0.1f, 2000.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMat4("projectionMat", projectionMatrixP);


    pLightShaderProgram->use();
    pLightShaderProgram->setMat4("projectionMat", projectionMatrixP);

    m_pLightSource = std::make_shared<RenderEngine::LightSource>(pLightShaderProgram, m_pDirLight0->direction);

    auto cubeMap = ResourceManager::getCubeMap("cubeMap2");
    m_pSkyBox = std::make_unique<RenderEngine::SkyBox>(cubeMap, pSkyBoxShaderProgram);

    pPlainMeshShaderProgram->use();
    pPlainMeshShaderProgram->setMat4("projectionMat", projectionMatrixP);
    m_pPlainMesh = std::make_unique<RenderEngine::PlainMesh>(pPlainMeshShaderProgram, 100, 100, 1.0f);

    p3DModelShaderProgram->use();
    p3DModelShaderProgram->setMat4("projectionMat", projectionMatrixP);
    p3DModelShaderProgram->setInt("skybox", 8);
    glActiveTexture(GL_TEXTURE8);
    cubeMap->bind();

    m_pTank = std::make_unique<Tank>(ResourceManager::getSprite("yellowTank_1"),
        glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.003f, 0.003f, 0.0f }, 0.1f, m_pLevel->getPlayerRespawn_1(),
        glm::vec2(Level::BLOCK_SIZE / 1.0, Level::BLOCK_SIZE / 1.0), 0.0f);

    m_pModel = ResourceManager::get3DModel("naruto");
    m_pModel1 = ResourceManager::get3DModel("cube");
    m_pCamera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));


    pTextShaderProgram->use();
    pTextShaderProgram->setMat4("projectionMat", projectionMatrixO);

    m_pLog = std::make_unique<RenderEngine::Text>(ResourceManager::getTexture("textArial256"), ResourceManager::getShaderProgram("textShader"));

    m_pPostProcess = std::make_unique<RenderEngine::PostProcess>(pPostProcessShaderProgram, m_windowSize);

    m_pLight = std::make_shared<RenderEngine::Light>();
    m_pLight->init();
    m_pLight->addDirLight(m_pDirLight0);
    m_pLight->addDirLight(m_pDirLight1);
    return true;
}

void Game::setWindowSize(const glm::ivec2 windowSize)
{
    m_windowSize = windowSize;
    m_pPostProcess->setWindowSize(m_windowSize);

    auto pTextShaderProgram = ResourceManager::getShaderProgram("textShader");
    glm::mat4 projectionMatrixO = 
        glm::ortho(0.0f, static_cast<float>(m_windowSize.x), 0.0f, static_cast<float>(m_windowSize.y), -100.0f, 100.0f);
    pTextShaderProgram->use();
    pTextShaderProgram->setMat4("projectionMat", projectionMatrixO);
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
        keyMouseRight(action);
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
    if (action == GLFW_PRESS)
    {
        auto p3DModelShaderProgram = ResourceManager::getShaderProgram("3DModelShader");
        if (m_blinn) m_blinn = 0;
        else m_blinn = 1;
    }
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
       m_pCamera->setSpeed(0.0008);
        break;
    case GLFW_RELEASE:
        m_pCamera->setSpeed(0.008);
        break;
    }
}
void Game::keyMouseRight(const int action)
{
    switch (action)
    {
    case GLFW_PRESS:
        m_lastCursorPos = m_currentCursorPos;
        m_pCamera->setSpeed(0.08);
        break;
    case GLFW_RELEASE:
        m_pCamera->setSpeed(0.008);
        break;
    }
}
