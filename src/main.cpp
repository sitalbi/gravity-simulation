#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "renderer/Shader.h"
#include "renderer/Renderer.h"
#include "renderer/models/baseModels/Cube.h"
#include "renderer/models/baseModels/Sphere.h"

#include "universe/Body.h"

#include "common/Camera.h"
#include "universe/Universe.h"

#define width 1920
#define height 1080

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

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();


    // Create cube
    Cube cube;

    Sphere sphere(1.0f, 20);

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 500.0f);

    // Create renderer
    Renderer renderer(Projection);

    // Create Camera
    Camera camera(window, Projection, glm::vec3(0, 0, 20), glm::vec3(0, 0, -1));

    // Create bodies with a sphere as model
    Body body1(glm::vec3(20.0f, 0.0f, 0.0f), sphere, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 25.0f, 0.4f);
    Body star(glm::vec3(0.0f, 0.0f, 0.0f), sphere, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.00f, 0.0f), 1000.0f, 100.0f,2.0f);

    // Create universe and add bodies
    Universe universe;
    universe.AddBody(&body1);
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

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Text("Application average  %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        // Edit the camera position in world space using ImGui
        ImGui::Text("Camera Position");
        ImGui::SliderFloat("X", &camera.m_position.x, -200.0f, 200.0f);
        ImGui::SliderFloat("Y", &camera.m_position.y, -200.0f, 200.0f);
        ImGui::SliderFloat("Z", &camera.m_position.z, -200.0f, 200.0f);

        // Edit the camera direction in world space using ImGui
        ImGui::Text("Camera Direction");
        ImGui::SliderFloat("Direction X", &camera.m_direction.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Direction Y", &camera.m_direction.y, -1.0f, 1.0f);
        ImGui::SliderFloat("Direction Z", &camera.m_direction.z, -1.0f, 1.0f);

        universe.Update();

        renderer.Draw(&body1, camera.getViewMatrix(), defaultShader);
        renderer.Draw(&star, camera.getViewMatrix(), starShader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
