#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common/loadShader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "common/Controls.hpp"

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

    std::vector<float> g_vertex_buffer_data = {
        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        // Right face
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,

        // Left face
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Top face
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f
    };


    std::vector<unsigned int> g_index_buffer_data = {
        0, 1, 2, 0, 2, 3,       // Front face
        4, 5, 6, 4, 6, 7,       // Back face
        8, 9, 10, 8, 10, 11,    // Right face
        12, 13, 14, 12, 14, 15, // Left face
        16, 17, 18, 16, 18, 19, // Top face
        20, 21, 22, 20, 22, 23  // Bottom face
    };

    std::vector<float> g_color_buffer_data = {
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


    // Vertex buffer
    unsigned int vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(float), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

    // Vertex array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);

    // Index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_data.size() * sizeof(unsigned int), &g_index_buffer_data[0], GL_STATIC_DRAW);

    // Color buffer
    unsigned int colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(float), &g_color_buffer_data[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);

    // Create controls
    Controls controls(window);

    // Projection matrix
    glm::mat4 Projection = controls.getProjectionMatrix();

    glm::mat4 View = controls.getViewMatrix();

    // Model matrix: an identity matrix 
    glm::mat4 Model = glm::mat4(1.0f);

    // Our ModelViewProjection: multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; 


    // Create and compile our GLSL program from the shaders
    unsigned int programID = LoadShaders("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");

    // Use our shader
    glUseProgram(programID);

    // Uniforms
    unsigned int matrixId = glGetUniformLocation(programID, "MVP");
    

    // Enable depth 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        controls.computeMatricesFromInputs();
        Projection = controls.getProjectionMatrix();
        View = controls.getViewMatrix();
        mvp = Projection * View * Model;

        // Send our transformation to the currently bound shader,
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        // Draw
        glDrawElements(GL_TRIANGLES, g_index_buffer_data.size(), GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}