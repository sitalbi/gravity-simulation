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
#include "renderer/Framebuffer.h"
#include "renderer/Skybox.h"
#include "renderer/models/baseModels/Cube.h"
#include "renderer/models/baseModels/Sphere.h"
#include "renderer/BloomRenderer.h"


#include "universe/Body.h"

#include "common/Camera.h"
#include "universe/Universe.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define width 1920
#define height 1080

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

std::vector<std::string> faces
{
    "res/textures/skybox/right.png",
    "res/textures/skybox/left.png",
    "res/textures/skybox/top.png",
    "res/textures/skybox/bottom.png",
    "res/textures/skybox/front.png",
    "res/textures/skybox/back.png"
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


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

    // Enable OpenGL debug output 
    glEnable(GL_DEBUG_OUTPUT);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
    ImGui_ImplOpenGL3_Init();

    // Create skybox
    Skybox skybox(faces);

    // Create cube
    Cube cube;

    Sphere sphere(1.0f, 20);

    // Projection matrix
    glm::mat4 Projection = glm::perspective(glm::radians(30.0f), (float)width / (float)height, 0.1f, 500.0f);

    // Create renderer
    Renderer renderer(Projection);

    glm::vec4 starColor(1.0f, 1.0f, 0.25f, 1.0f);

    // Create bodies with a sphere as model
    Body body1(glm::vec3(20.0f, 0.0f, 0.0f), sphere, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 15.0f, 0.4f, 0.5f);
    Body body2(glm::vec3(30.0f, 0.0f, 0.0f), sphere, glm::vec4(0.75f, 0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 10.0f, 0.5f, 0.35f);
    Body body3(glm::vec3(40.0f, 0.0f, 0.0f), sphere, glm::vec4(0.0f, 0.75f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.055f), 10.0f, 0.5f, 0.35f);
    Body star(glm::vec3(0.0f, 0.0f, 0.0f), sphere, starColor, glm::vec3(0.0f, 0.00f, 0.0f), 1000.0f, 100.0f,2.0f);

    body1.SetName("Planet 1");
    body2.SetName("Planet 2");
    body3.SetName("Planet 3");
    star.SetName("Star");

    // Create Camera
    Camera camera(window, Projection, glm::vec3(0, 0, 50), star.GetPosition(), width, height);

    glfwSetWindowUserPointer(window, &camera);

    // Create universe and add bodies
    Universe universe;
    universe.AddBody(&body1);
    universe.AddBody(&body2);
    universe.AddBody(&body3);
    universe.SetEmissiveBody(&star);

    // Create, compile and bind shader
    Shader starShader("res/shaders/star/VertexShader.glsl", "res/shaders/star/FragmentShader.glsl");
    Shader defaultShader("res/shaders/default/VertexShader.glsl", "res/shaders/default/FragmentShader.glsl");
    Shader skyboxShader("res/shaders/skybox/VertexShader.glsl", "res/shaders/skybox/FragmentShader.glsl");
    Shader screenShader("res/shaders/postProcessing/VertexShader.glsl", "res/shaders/postProcessing/FragmentShader.glsl");
    
    // Check for OpenGL errors after shader creation
    GLenum shaderErr = glGetError();
    if (shaderErr != GL_NO_ERROR)
    {
        std::cerr << "Shader Creation Error: " << shaderErr << std::endl;
        return -1;
    }
    
    // Set shaders uniforms
    defaultShader.Bind();
    defaultShader.SetUniform4f("u_LightColor",1.0f, 1.0f, 1.0f, 1.0f);
    defaultShader.SetUniform4f("u_LightPosition",star.GetPosition().x, star.GetPosition().y, star.GetPosition().z, 1.0f);

    skyboxShader.Bind();
    skyboxShader.SetUniform1i("u_Skybox", 0);

    // Create HDR framebuffer
    Framebuffer hdrFBO(width, height, 2);

    // Create Bloom renderer
    BloomRenderer bloomRenderer;
    bloomRenderer.Init(width, height, renderer, 5);

    // Set draw buffers
    const GLenum drawBuffersAll[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    const GLenum drawBuffer0[] = { GL_COLOR_ATTACHMENT0 };
    const GLenum drawBuffer1[] = { GL_COLOR_ATTACHMENT1 };

    // Enable depth 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    float starExposure = 1.0f;
    float filterRadius = 0.005f;
    float bloomIntensity = 1.0f;
    int mipNumber = 1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Text("Application average  %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Begin("Focus");
        ImGui::Text("Select the body to focus on:");

        // Loop to create checkboxes for each body in the universe
        for (unsigned int i = 0; i < universe.bodies.size(); i++)
        {
            std::string checkboxLabel = universe.bodies[i]->GetName();

            // Check if the checkbox is checked
            bool checked = (universe.GetFocusedBodyId() == i);

            if (ImGui::Checkbox(checkboxLabel.c_str(), &checked))
            {
                if (checked)
                {
                    universe.SetFocusedBody(i);
                    camera.m_distance = universe.bodies[i]->GetRadius() * 10.0f;
                }
            }
        }
        ImGui::End();
        
        ImGui::Begin("Star");
        ImGui::SliderFloat("Exposure", &starExposure, 1.0f, 10.0f);
        ImGui::SliderFloat("Filter Radius", &filterRadius, 0.001f, 0.1f);
        ImGui::ColorEdit3("Star Color", &star.m_color.r);
        ImGui::End();

        // Render to HDR framebuffer
        hdrFBO.Bind();
        renderer.Clear();

        // Update universe's bodies
        universe.Update();

        // Test if mouse pressed and not on ImGui window
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ImGui::IsAnyItemHovered())
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

        glDrawBuffers(1, drawBuffer0);
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw bodies in hdrFBO color buffer 0
        for (unsigned int i = 0; i < universe.bodies.size(); i++)
        {
            if (i != universe.GetEmissiveBodyId())
            {
                defaultShader.Bind();
                glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), universe.bodies[i]->GetPosition());
                defaultShader.SetUniformMat4f("u_M", modelMat);
                renderer.Draw(universe.bodies[i], camera.GetViewMatrix(), defaultShader);
            } 
		}
        // Render skybox in color buffer 0
        skyboxShader.Bind();
        skybox.Draw(camera.GetViewMatrix(), camera.GetProjectionMatrix(), skyboxShader);
        

        glDrawBuffers(2, drawBuffersAll);

        // Render emissive body in both color buffers
        starShader.Bind();
        renderer.Draw(&star, camera.GetViewMatrix(), starShader);
        hdrFBO.Unbind();

        // Bloom effect
        bloomRenderer.RenderBloomTexture(hdrFBO.GetTextureColorBuffer(1), filterRadius);
        
        // Render to screen
        screenShader.Bind();
        screenShader.SetUniform1i("scene", 0);
        screenShader.SetUniform1i("bloomBlur", 1);
        screenShader.SetUniform1f("bloomExposure", starExposure);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrFBO.GetTextureColorBuffer(0)); // Scene texture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, bloomRenderer.BloomTexture()); // Bloom texture

        renderer.DrawQuad();


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
    bloomRenderer.Destroy();
    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (camera->m_distance - yoffset > 1.0f) {
        camera->m_distance -= yoffset;
    }
}
