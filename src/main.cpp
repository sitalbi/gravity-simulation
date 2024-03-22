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
#include "renderer/models/baseModels/Sphere.h"

#include "universe/Body.h"

#include "common/Camera.h"
#include "universe/Universe.h"

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

    Sphere sphere(1.0f, 20);

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(
		glm::vec3(1, 0, 100), // Camera position in World Space
		glm::vec3(0, 0, 0), // Looks at the origin
		glm::vec3(0, 1, 0)  // Head is up
	);

    // Create renderer
    Renderer renderer(Projection);

    // Create Camera
    Camera camera(window, Projection, glm::vec3(1, 0, 10));

    // Create bodies with a cube as model
    Body body1(glm::vec3(20.0f, 0.0f, 0.0f), sphere, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.055f, 0.0f), 25.0f, 0.4f);
    //Body body2(glm::vec3(15.0f, 0.0f, 0.0f), &sphere, glm::vec4(0.5f, 0.0f, 0.5f, 1.0f), glm::vec3(0.0f, 0.075f, 0.0f), 100.0f, 2.0f);
    Body star(glm::vec3(0.0f, 0.0f, 0.0f), sphere, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.00f, 0.0f), 1000.0f, 100.0f);
    //Body body2(glm::vec3(1.0f, 1.0f, 0.0f), &sphere, glm::vec4(0.5f, 0.0f, 0.5f, 1.0f));
    //Body body3(glm::vec3(3.0f, 2.0f, 1.0f), &sphere, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));

    // Create universe and add bodies
    Universe universe;
    
    universe.AddBody(&body1);
    //universe.AddBody(&body2);
    universe.SetEmissiveBody(&star);

    // Create, compile and bind shader
    Shader starShader("res/shaders/star/VertexShader.glsl", "res/shaders/star/FragmentShader.glsl");
    Shader defaultShader("res/shaders/default/VertexShader.glsl", "res/shaders/default/FragmentShader.glsl");

    // Check for OpenGL errors after shader creation
    GLenum shaderErr = glGetError();
    if (shaderErr != GL_NO_ERROR)
    {
        std::cerr << "Shader Creation Error: " << shaderErr << std::endl;
        return -1;
    }

    // Set light position vector
    glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 10.0f);
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f));
    
    // Set Fragment shader uniforms
    defaultShader.Bind();
    defaultShader.SetUniform4f("u_LightColor", star.GetColor().x, star.GetColor().y, star.GetColor().z, star.GetColor().w);
    defaultShader.SetUniform4f("u_LightPosition",star.GetPosition().x, star.GetPosition().y, star.GetPosition().z, 1.0f);

    // Enable depth 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        camera.computeMatricesFromInputs();
        View = camera.getViewMatrix();

        universe.Update();

        defaultShader.Bind();
        defaultShader.SetUniform3f("u_viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

        renderer.Draw(&body1, View, defaultShader);
        //renderer.Draw(&body2, View, shader);
        renderer.Draw(&star, View, starShader);

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
