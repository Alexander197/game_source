#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

glm::ivec2 g_windowSize(680, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) 
{
    g_windowSize.x = width;  
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
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
        std::cout<<"gldwCreateWindow failed!"<<std::endl;
        glfwTerminate();
        return -1;
    }
   
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    glfwMakeContextCurrent(pWindow);
	
	if(!gladLoadGL())
	{
		std::cout<<"Can't load GLAD!"<<std::endl;
		return -1;
	}
    std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<std::endl;
    std::cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<std::endl;
	
	glClearColor(1,1,1,1);
    {
        ResourceManager resourceManager(argv[0]);

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");

        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create sprite shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/brick.jpg");
        std::vector<std::string> subTextureNames = {"tex1","tex2","tex3","tex4","tex5","tex6","tex7","tex8"};

        auto textureAtlas = resourceManager.loadTextureAtlas("DafaultTextureAtlas", "res/textures/different_textures.jpg", subTextureNames, 100, 100);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTexture", "SpriteShader", 50, 50);
        auto pTex1Sprite = resourceManager.loadSprite("Tex1Sprite", "DafaultTextureAtlas", "SpriteShader", 100, 100, "tex1");

        glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(g_windowSize.x), 0.0f, static_cast<float>(g_windowSize.y), -100.0f, 100.0f);
        
        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            
            pSprite->setPosition(glm::vec2(100, 100));
            pSprite->render();

            pTex1Sprite->setPosition(glm::vec2(300, 300));
            pTex1Sprite->render();


            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);
            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}