#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include <cmath>
#include <vector>
#include <string>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"

#include "Renderer/Renderer.h"




#define ASSERT(x) if (!(x)) __debugbreak(); // specific for msvc
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
}

glm::ivec2 g_windowSize(640, 480);

Game g_game(g_windowSize);


// Window size callback
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    
    // show OpenGL where to draw
    //glViewport(0, 0, g_windowSize.x, g_windowSize.y);
    RendererEngine::Renderer::setViewport(width, height);

}

 
// Keypress callback
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_game.setKey(key, action);
}

int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed!" << std::endl;
        return -1;
    }

    // Cannot create window when OpenGL version is less than 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    
    // Turning off unnecessaary backward compatibillity features 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "engine", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Callbacks registration
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    // Displaying OpenGL version
    std::cout << "Renderer: " << RendererEngine::Renderer::getRendererStr() << std::endl;
    std::cout << "OpenGL version: " << RendererEngine::Renderer::getVersionStr() << std::endl;
    std::cout << "executablePath argv[0]: " << argv[0] << std::endl;
    

    //glClearColor(0, 0, 0, 1);    
    RendererEngine::Renderer::setClearColor(0, 0, 0, 1);


    // Creating scope because we need delete ShaderPrograms before deleting glContext???
    {
        // ResourceManager initialize
        ResourceManager::setExecutablePath(argv[0]);
        g_game.init();

        auto lastTime = std::chrono::high_resolution_clock::now();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Poll for and process events */
            glfwPollEvents();
            


            auto currentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            g_game.update(duration);



            /* Render here */
            RendererEngine::Renderer::clear();
            //glClear(GL_COLOR_BUFFER_BIT);
         
            g_game.render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

        }
        ResourceManager::unloadResources();
    }
    glfwTerminate();
    return 0;
}