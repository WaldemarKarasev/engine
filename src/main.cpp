#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

GLfloat point[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
};

// OpenSL language???
const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"   color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"   frag_color = vec4(color, 1.0f);"
"};";

int g_windowSizeX = 640;
int g_windowSizeY = 480;

// Window size callback
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    
    // show OpenGL where to draw
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);

}


// Keypress callback
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
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
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "engine", nullptr, nullptr);
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
    
    glClearColor(1, 1, 0, 1);    

    // Creating shaders
    // Vertex shader
    // Shader identifier
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    // Compiling vertex shader
    glCompileShader(vs);

    // Fragment shader
    // Shader identifier
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    // Compiling fragment shader
    glCompileShader(fs);

    // Linking vertex and fragment shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    // Deleting vs and fs
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Passing shader_program to video card
    // VBO creating (Vertex Buffer Object)
    // Creating buffer for points data
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    // Activate points_vbo 
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    // Fill data in points_vbo // passing data from RAM to Video Card memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    // Creating buffer for colors data
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    // Activate colors_vbo 
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    // Fill data in colors_vbo // passing data from RAM to Video Card memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


    // Linking buffers data and shader???
    // Creating VAO (Vertex Attribute {???or Array???} Object)
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Link VBO and VAO
    // for points
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    // for colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawwing triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}