#include "glad.h"
#include "glfw3.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ------------------------------------------------------------
// Vertex Shader
// ------------------------------------------------------------
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

// ------------------------------------------------------------
// Fragment Shader - Cyan Color
// ------------------------------------------------------------
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";


int main()
{
    // --------------------------------------------------------
    // Initialize GLFW
    // --------------------------------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // --------------------------------------------------------
    // Create Window
    // Window title = Full ID
    // --------------------------------------------------------
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "0432410005101017",
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

    // --------------------------------------------------------
    // Initialize GLAD
    // --------------------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // --------------------------------------------------------
    // Create Vertex Shader
    // --------------------------------------------------------
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);

    // --------------------------------------------------------
    // Create Fragment Shader
    // --------------------------------------------------------
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL
    );

    glCompileShader(fragmentShader);

    // --------------------------------------------------------
    // Create Shader Program
    // --------------------------------------------------------
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    // Shaders no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ========================================================
    // STAR VERTICES
    // ========================================================
    //
    // A 5-point star has:
    // 5 outer points
    // 5 inner points
    //
    // Total = 10 points
    //
    // We create triangles from the center to each
    // consecutive pair of points.
    //
    // This makes a complete star using GL_TRIANGLES.
    // ========================================================

    const float PI = 3.14159265359f;

    float outerRadius = 0.75f;
    float innerRadius = 0.32f;

    // Center point
    float centerX = 0.0f;
    float centerY = 0.0f;

    float starVertices[10 * 9];

    int index = 0;

    // Start angle = 90 degrees (top)
    float startAngle = PI / 2.0f;

    // Create 10 triangles
    for (int i = 0; i < 10; i++)
    {
        // Current point
        float angle1 =
            startAngle + i * PI / 5.0f;

        // Next point
        float angle2 =
            startAngle + (i + 1) * PI / 5.0f;

        // Alternate between outer and inner radius
        float radius1;

        if (i % 2 == 0)
            radius1 = outerRadius;
        else
            radius1 = innerRadius;

        float radius2;

        if ((i + 1) % 2 == 0)
            radius2 = outerRadius;
        else
            radius2 = innerRadius;

        // ----------------------------------------------------
        // Center vertex
        // ----------------------------------------------------
        starVertices[index++] = centerX;
        starVertices[index++] = centerY;
        starVertices[index++] = 0.0f;

        // ----------------------------------------------------
        // Current star point
        // ----------------------------------------------------
        starVertices[index++] =
            radius1 * cos(angle1);

        starVertices[index++] =
            radius1 * sin(angle1);

        starVertices[index++] = 0.0f;

        // ----------------------------------------------------
        // Next star point
        // ----------------------------------------------------
        starVertices[index++] =
            radius2 * cos(angle2);

        starVertices[index++] =
            radius2 * sin(angle2);

        starVertices[index++] = 0.0f;
    }


    // ========================================================
    // VAO and VBO
    // ========================================================

    unsigned int VAO;
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy star vertex data to GPU
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(starVertices),
        starVertices,
        GL_STATIC_DRAW
    );

    // --------------------------------------------------------
    // Vertex Attribute
    // --------------------------------------------------------
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ========================================================
    // Render Loop
    // ========================================================

    while (!glfwWindowShouldClose(window))
    {
        // ----------------------------------------------------
        // Input
        // Press A to close window
        // ----------------------------------------------------
        processInput(window);

        // ----------------------------------------------------
        // Yellow Background
        // RGB = 1.0, 1.0, 0.0
        // ----------------------------------------------------
        glClearColor(
            1.0f,
            1.0f,
            0.0f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);

        // ----------------------------------------------------
        // Use Cyan Shader
        // ----------------------------------------------------
        glUseProgram(shaderProgram);

        // Bind Star
        glBindVertexArray(VAO);

        // ----------------------------------------------------
        // Draw Complete Star
        //
        // 10 triangles
        // 3 vertices per triangle
        // Total = 30 vertices
        // ----------------------------------------------------
        glDrawArrays(
            GL_TRIANGLES,
            0,
            30
        );

        // ----------------------------------------------------
        // Swap buffers
        // ----------------------------------------------------
        glfwSwapBuffers(window);

        // Process events
        glfwPollEvents();
    }


    // ========================================================
    // Cleanup
    // ========================================================

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}


// ============================================================
// Process Input
// Press 'A' to close the window
// ============================================================

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


// ============================================================
// Window Resize Callback
// ============================================================

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