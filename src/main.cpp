#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "Resources/stb_image.h"

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

GLfloat point[] = {
     0.0f,   50.f, 0.f,
     50.0f, -50.f, 0.f,
    -50.f,  -50.f, 0.f

     //50.0f,  50.f, /*0.f,*/
     //50.f,  0.0f, /*0.f,*/
     //0.0f, 0.0f/*, 0.f*/
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};


glm::ivec2 g_windowSize(640, 480);

// Window size callback
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    
    // show OpenGL where to draw
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);

}

 
// Keypress callback
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
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
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "executablePath argv[0]: " << argv[0] << std::endl;
    
    glClearColor(0, 1, 1, 1);    

    // Creating scope because we need delete ShaderPrograms before deleting glContext???
    {
        // ResourceManager initialize
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }


        // Sprite initializing START
        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        // subtextures list
        std::vector<std::string> subTextureNames = { "0", "1", "2", "3", "4", "5", "6"};
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "0");
        pSprite->setPosition(glm::vec2(300, 140));
        //pSprite->setRotation(45.0f);
        // Sprite initializing END




        
         //Passing shader_program to video card
         //VBO creating (Vertex Buffer Object)
         //Creating buffer for points data
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
         //Activate points_vbo 
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
         //Fill data in points_vbo // passing data from RAM to Video Card memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

         //Creating buffer for colors data
        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        // Activate colors_vbo 
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
         //Fill data in colors_vbo // passing data from RAM to Video Card memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

         //Creating buffer for texture data
        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
         //Activate texture_vbo 
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
         //Fill data in texture_vbo // passing data from RAM to Video Card memory
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);


         //Linking buffers data and shader???
         //Creating VAO (Vertex Attribute {???or Array???} Object)
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //Link VBO and VAO
         //for points
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        // for colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        // for textures
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        
       
        

        // Transformation matrix
        // for the 1st triangle
        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));
        
        // for the 2nd triangle
        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

        // orthographic projection matrix
        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);
        
        
        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);
        
        
        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);
        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            

            
            pSprite->render();

            
            // triangle render
            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}