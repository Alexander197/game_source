#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/matrix.hpp>

#include <iostream>
#include <vector>
#include <chrono>

#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"

#include "Game/Game.h"

glm::ivec2 g_windowSize(1920, 1080);

std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;  
    g_windowSize.y = height;

    const float aspect_ratio = static_cast<float>(g_game->getCurrentLevelWidth()) / g_game->getCurrentLevelHeight();

    unsigned int viewPortWidth = width;
    unsigned int viewPortHeight = height;

    unsigned int viewPortLeftOffset = 0.0f;
    unsigned int viewPortBottomOffset = 0.0f;

    if (static_cast<float>(width) / height > aspect_ratio)
    {
        viewPortWidth = height * aspect_ratio;
        viewPortLeftOffset = (width - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = width / aspect_ratio;
        viewPortBottomOffset = (height - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game->setKey(key, action);
}
void glfwMouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    g_game->setMouseButton(button, action);
}
void glfwCursorPosCallback(GLFWwindow* pWindow, double xpos, double ypos)
{
    g_game->setCursorPos(xpos, ypos);
}
void glfwScrollCallback(GLFWwindow*, double xoffset, double yoffset)
{
    g_game->setScrollOffset(xoffset, yoffset);
}


int main(int argc, char** argv)
{
    if (!glfwInit()) {
        std::cout << "glwInit failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Hello World", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout<<"glfwCreateWindow failed!"<<std::endl;
        glfwTerminate();
        return -1;
    }
   
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwSetMouseButtonCallback(pWindow, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(pWindow, glfwCursorPosCallback);
    glfwSetScrollCallback(pWindow, glfwScrollCallback);

    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL())
	{
		std::cout<<"Can't load GLAD!"<<std::endl;
		return -1;
	}
    std::cout<<"Renderer: "<<RenderEngine::Renderer::getRendererStr()<<std::endl;
    std::cout<<"OpenGL version: "<<RenderEngine::Renderer::getVersionStr()<<std::endl;
	
    RenderEngine::Renderer::setClearColor(0.0, 0.0, 0.0, 1);
    RenderEngine::Renderer::setDepthTest(true);

    {
        ResourceManager::setExecutablePath(argv[0]);
        g_game->init();
        g_windowSize.x = static_cast<int>(g_game->getCurrentLevelWidth());
        g_windowSize.y = static_cast<int>(g_game->getCurrentLevelHeight());
        glfwSetWindowSize(pWindow, g_windowSize.x, g_windowSize.y);

        auto lastTime = std::chrono::high_resolution_clock::now();
 
        float fps = 0.0f;
        int counter = 0;

        glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        while (!glfwWindowShouldClose(pWindow))
        {
            /* Poll for and process events */
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count(); 

            fps += duration;
            counter++;
            if (counter == 20)
            {
                float f = 1 / (fps / counter) * 1000.0;
                std::cout << f << std::endl;      
                g_game->setFPS(f);
                fps = 0.0f;
                counter = 0;
            }
            lastTime = currentTime;
            g_game->update(duration);

            RenderEngine::Renderer::clear();
            /* Render here */

            g_game->render();

            /* Render here */
            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);
        }
        g_game = nullptr;
        ResourceManager::unloadAllResources();
    }
    glfwTerminate();
    return 0;
}