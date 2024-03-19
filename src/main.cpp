#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer/Shader.h"
#include "renderer/Renderer.h"
#include "renderer/models/baseModels/Cube.h"

#include "universe/Body.h"

#include "common/Controls.h"

#define width 1280
#define height 720

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = 0x" << std::hex << type << ", severity = 0x" << std::hex << severity << ", message = " << message << std::endl;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Set GLFW error callback */
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error: " << description << std::endl;
        });

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Gravity simulation", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLEW */
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    /* Enable OpenGL debug output */
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);


    // Create cube
    Cube cube;

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 View;

    // Create renderer
    Renderer renderer(Projection);

    // Create controls
    Controls controls(window, Projection);

    // Create bodies with a cube as model
    Body body1(glm::vec3(0.0f, 0.0f, 0.0f), &cube);
    Body body2(glm::vec3(2.0f, 1.0f, 0.0f), &cube);
    Body body3(glm::vec3(4.0f, 2.0f, 0.0f), &cube);

    // Create, compile and bind shader
    Shader shader("res/shaders/VertexShader.glsl", "res/shaders/FragmentShader.glsl");

    // Check for OpenGL errors after shader creation
    GLenum shaderErr = glGetError();
    if (shaderErr != GL_NO_ERROR)
    {
        std::cerr << "Shader Creation Error: " << shaderErr << std::endl;
        return -1;
    }

    // Set light position vector
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 10.0f);
    
    // Set Fragment shader uniforms
    shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.5f, 1.0f);
    shader.SetUniform4f("u_LightColor", 1.0f, 1.0f, 1.0f, 1.0f);
    shader.SetUniform4f("u_LightPosition", lightPos.x, lightPos.y, lightPos.z, 1.0f);
    shader.SetUniform1f("u_LightPower", 200.0f);

    // Enable depth 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        controls.computeMatricesFromInputs();
        View = controls.getViewMatrix();


        renderer.Draw(&body1, View, shader);
        renderer.Draw(&body2, View, shader);
        renderer.Draw(&body3, View, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        // Check for OpenGL errors after each frame
        GLenum frameErr = glGetError();
        if (frameErr != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error: " << frameErr << std::endl;
            return -1;
        }
    }

    glfwTerminate();
    return 0;
}
