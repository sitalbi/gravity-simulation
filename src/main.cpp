#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common/loadShader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "common/Controls.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#define width 1280
#define height 720

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewInit();

    std::vector<float> g_VertexBufferData = {
        // Front face
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        // Right face
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

        // Left face
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Top face
        -0.5f, 0.5f,  0.5f,
         0.5f, 0.5f,  0.5f,
         0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f
    };


    std::vector<unsigned int> g_IndexBufferData = {
        0, 1, 2, 0, 2, 3,       // Front face
        4, 5, 6, 4, 6, 7,       // Back face
        8, 9, 10, 8, 10, 11,    // Right face
        12, 13, 14, 12, 14, 15, // Left face
        16, 17, 18, 16, 18, 19, // Top face
        20, 21, 22, 20, 22, 23  // Bottom face
    };

    std::vector<float> g_ColorBufferData = {
        // Front face
        1.0f, 0.0f, 0.0f, // Red
        1.0f, 0.0f, 0.0f, // Red
        1.0f, 0.0f, 0.0f, // Red
        1.0f, 0.0f, 0.0f, // Red

        // Back face
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 1.0f, 0.0f, // Green
        0.0f, 1.0f, 0.0f, // Green

        // Right face
        0.0f, 0.0f, 1.0f, // Blue
        0.0f, 0.0f, 1.0f, // Blue
        0.0f, 0.0f, 1.0f, // Blue
        0.0f, 0.0f, 1.0f, // Blue

        // Left face
        1.0f, 1.0f, 0.0f, // Yellow
        1.0f, 1.0f, 0.0f, // Yellow
        1.0f, 1.0f, 0.0f, // Yellow
        1.0f, 1.0f, 0.0f, // Yellow

        // Top face
        0.0f, 1.0f, 1.0f, // Cyan
        0.0f, 1.0f, 1.0f, // Cyan
        0.0f, 1.0f, 1.0f, // Cyan
        0.0f, 1.0f, 1.0f, // Cyan

        // Bottom face
        1.0f, 0.0f, 1.0f, // Magenta
        1.0f, 0.0f, 1.0f, // Magenta
        1.0f, 0.0f, 1.0f, // Magenta
        1.0f, 0.0f, 1.0f, // Magenta
    };

    // Create and bind vertex array
    VertexArray va;

    // Create and bind vertex buffer
    VertexBuffer vb(&g_VertexBufferData[0], g_VertexBufferData.size() * sizeof(float));

    // Vertex buffer layout
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 3);
    va.AddBuffer(vb, layout);

    // Create and bind index buffer
    IndexBuffer ib(&g_IndexBufferData[0], g_IndexBufferData.size());

    // Create and bind color buffer
    VertexBuffer cb(&g_ColorBufferData[0], g_ColorBufferData.size() * sizeof(float));

    // Color buffer layout
    layout.Push(GL_FLOAT, 3);
    va.AddBuffer(cb, layout);


    // ModelViewProjection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix: an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection: multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model;

    // Create and compile shaders
    unsigned int programID = LoadShaders("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");

    // Use our shader
    glUseProgram(programID);

    // Shader uniforms
    unsigned int matrixId = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
    

    // Enable depth 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw
        glDrawElements(GL_TRIANGLES, g_IndexBufferData.size(), GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}