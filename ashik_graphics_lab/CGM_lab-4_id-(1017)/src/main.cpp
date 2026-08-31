#include "glad.h"
#include "glfw3.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Vertex Shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";


// Cyan Fragment Shader
const char *fragmentShaderCyanSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";


// Magenta Fragment Shader
const char *fragmentShaderMagentaSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // Create Window
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Ashiqujjaman Sarker",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );


    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // -----------------------------------
    // Create Shaders
    // -----------------------------------

    unsigned int vertexShader;
    unsigned int fragmentShaderCyan;
    unsigned int fragmentShaderMagenta;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderCyan = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderMagenta = glCreateShader(GL_FRAGMENT_SHADER);


    // Vertex Shader
    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);


    // Cyan Fragment Shader
    glShaderSource(
        fragmentShaderCyan,
        1,
        &fragmentShaderCyanSource,
        NULL
    );

    glCompileShader(fragmentShaderCyan);


    // Magenta Fragment Shader
    glShaderSource(
        fragmentShaderMagenta,
        1,
        &fragmentShaderMagentaSource,
        NULL
    );

    glCompileShader(fragmentShaderMagenta);


    // -----------------------------------
    // Cyan Shader Program
    // -----------------------------------

    unsigned int shaderProgramCyan = glCreateProgram();

    glAttachShader(
        shaderProgramCyan,
        vertexShader
    );

    glAttachShader(
        shaderProgramCyan,
        fragmentShaderCyan
    );

    glLinkProgram(shaderProgramCyan);


    // -----------------------------------
    // Magenta Shader Program
    // -----------------------------------

    unsigned int shaderProgramMagenta = glCreateProgram();

    glAttachShader(
        shaderProgramMagenta,
        vertexShader
    );

    glAttachShader(
        shaderProgramMagenta,
        fragmentShaderMagenta
    );

    glLinkProgram(shaderProgramMagenta);


    // -----------------------------------
    // Cyan Square
    // -----------------------------------

    float square[] =
    {
        // First triangle
        -0.6f, -0.5f, 0.0f,
         0.6f, -0.5f, 0.0f,
         0.6f,  0.5f, 0.0f,

        // Second triangle
        -0.6f, -0.5f, 0.0f,
         0.6f,  0.5f, 0.0f,
        -0.6f,  0.5f, 0.0f
    };


    // -----------------------------------
    // Magenta Triangle
    // -----------------------------------
    // Shares the top-left and top-right
    // corners of the square

    float triangle[] =
    {
        -0.6f,  0.5f, 0.0f,   // Top-left corner
         0.6f,  0.5f, 0.0f,   // Top-right corner
         0.0f,  0.9f, 0.0f    // Top point
    };


    // -----------------------------------
    // VAO & VBO
    // -----------------------------------

    unsigned int VAO[2];
    unsigned int VBO[2];

    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);


    // -----------------------------------
    // Square Setup
    // -----------------------------------

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(square),
        square,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // -----------------------------------
    // Triangle Setup
    // -----------------------------------

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(triangle),
        triangle,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // -----------------------------------
    // Render Loop
    // -----------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // Check keyboard input
        processInput(window);


        // White Background
        glClearColor(
            1.0f,
            1.0f,
            1.0f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);


        // -----------------------------------
        // Draw Cyan Square
        // -----------------------------------

        glUseProgram(shaderProgramCyan);

        glBindVertexArray(VAO[0]);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            6
        );


        // -----------------------------------
        // Draw Magenta Triangle
        // -----------------------------------

        glUseProgram(shaderProgramMagenta);

        glBindVertexArray(VAO[1]);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            3
        );


        // Display
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // -----------------------------------
    // Cleanup
    // -----------------------------------

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    glDeleteProgram(shaderProgramCyan);
    glDeleteProgram(shaderProgramMagenta);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderCyan);
    glDeleteShader(fragmentShaderMagenta);

    glfwTerminate();

    return 0;
}


// -----------------------------------
// Keyboard Input
// -----------------------------------

void processInput(GLFWwindow *window)
{
    // Press A to close the program
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // ESC can also close the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


// -----------------------------------
// Window Resize Callback
// -----------------------------------

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(
        0,
        0,
        width,
        height
    );
}