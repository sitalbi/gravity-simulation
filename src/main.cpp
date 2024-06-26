#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define width 1920
#define height 1080

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

unsigned int loadCubemap(std::vector<std::string> faces);

std::vector<float> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
    "res/textures/skybox/right.jpg",
    "res/textures/skybox/left.jpg",
    "res/textures/skybox/top.jpg",
    "res/textures/skybox/bottom.jpg",
    "res/textures/skybox/front.jpg",
    "res/textures/skybox/back.jpg"
};

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

    glfwSetScrollCallback(window, scroll_callback);

    /* Enable OpenGL debug output */
    glEnable(GL_DEBUG_OUTPUT);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    
    unsigned int cubemapTexture = loadCubemap(faces);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // Create cube
    Cube cube;

    Sphere sphere(1.0f, 20);

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 500.0f);

    // Create renderer
    Renderer renderer(Projection);

    // Create bodies with a sphere as model
    Body body1(glm::vec3(20.0f, 0.0f, 0.0f), sphere, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 15.0f, 0.4f, 0.5f);
    Body body2(glm::vec3(30.0f, 0.0f, 0.0f), sphere, glm::vec4(0.5f, 0.0f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 10.0f, 0.5f, 0.35f);
    Body star(glm::vec3(0.0f, 0.0f, 0.0f), sphere, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f), glm::vec3(0.0f, 0.00f, 0.0f), 1000.0f, 100.0f,2.0f);

    body1.SetName("Planet 1");
    body2.SetName("Planet 2");
    star.SetName("Star");

    // Create Camera
    Camera camera(window, Projection, glm::vec3(0, 0, 50), star.GetPosition(), width, height);

    glfwSetWindowUserPointer(window, &camera);

    // Create universe and add bodies
    Universe universe;
    universe.AddBody(&body1);
    universe.AddBody(&body2);
    universe.SetEmissiveBody(&star);

    // Create, compile and bind shader
    Shader starShader("res/shaders/star/VertexShader.glsl", "res/shaders/star/FragmentShader.glsl");
    Shader defaultShader("res/shaders/default/VertexShader.glsl", "res/shaders/default/FragmentShader.glsl");
    Shader skyboxShader("res/shaders/skybox/VertexShader.glsl", "res/shaders/skybox/FragmentShader.glsl");

    // Check for OpenGL errors after shader creation
    GLenum shaderErr = glGetError();
    if (shaderErr != GL_NO_ERROR)
    {
        std::cerr << "Shader Creation Error: " << shaderErr << std::endl;
        return -1;
    }
    
    // Set shaders uniforms
    defaultShader.Bind();
    defaultShader.SetUniform4f("u_LightColor", star.GetColor().x, star.GetColor().y, star.GetColor().z, star.GetColor().w);
    defaultShader.SetUniform4f("u_LightPosition",star.GetPosition().x, star.GetPosition().y, star.GetPosition().z, 1.0f);

    skyboxShader.Bind();
    skyboxShader.SetUniform1i("u_Skybox", 0);

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

        ImGui::Begin("Focus");
        ImGui::Text("Select the body to focus on:");

        // Loop to create checkboxes for each body in the universe
        for (unsigned int i = 0; i < universe.bodies.size(); i++)
        {
            // Create a unique label for each checkbox
            std::string checkboxLabel = universe.bodies[i]->GetName();

            // Check if the checkbox is checked
            bool checked = (universe.GetFocusedBodyId() == i);

            // Create the checkbox and set its state
            if (ImGui::Checkbox(checkboxLabel.c_str(), &checked))
            {
                // If the checkbox state changed, update the focused body
                if (checked)
                {
                    universe.SetFocusedBody(i);
                }
            }
        }
        ImGui::End();

        // Update universe's bodies
        universe.Update();

        // Test if mouse pressed
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            camera.RotateCamera(glm::vec2(xpos, ypos));
        }
        else {
            camera.ResetDrag();
        }

        Body* focusedBody = universe.bodies[universe.GetFocusedBodyId()];

        // Look at focused body
        camera.SetLookAt(focusedBody->GetPosition());
        camera.m_position = focusedBody->GetPosition() + camera.m_distance * glm::normalize(focusedBody->GetPosition() - camera.GetOrbitPosition());

        // Draw bodies
        for (unsigned int i = 0; i < universe.bodies.size(); i++)
        {
            if (i == universe.GetEmissiveBodyId())
            {
                renderer.Draw(universe.bodies[i], camera.GetViewMatrix(), starShader);
            }
            else
            {
                defaultShader.Bind();
                glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), universe.bodies[i]->GetPosition());
                defaultShader.SetUniformMat4f("u_M", modelMat);
                renderer.Draw(universe.bodies[i], camera.GetViewMatrix(), defaultShader);
            }
		}


        // Render skybox
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Bind();
        skyboxShader.SetUniformMat4f("u_View", glm::mat4(glm::mat3(camera.GetViewMatrix())));
        skyboxShader.SetUniformMat4f("u_Projection", camera.GetProjectionMatrix());
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);


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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (camera->m_distance - yoffset > 1.0f) {
        camera->m_distance -= yoffset;
    }
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int w, h, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}