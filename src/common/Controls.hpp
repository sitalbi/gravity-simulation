#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Controls
{
public:
	Controls(GLFWwindow* window);

	void computeMatricesFromInputs();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private:
	GLFWwindow* window;

	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 position;

	float horizontalAngle;
	float verticalAngle;
	float speed;
	float mouseSpeed;

	double xpos;
	double ypos;

	int width;
	int height;

	float deltaTime;
	float lastTime;
	double currentTime;
};