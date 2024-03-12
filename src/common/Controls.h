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
	GLFWwindow* m_window;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::vec3 m_position;

	float m_horizontalAngle;
	float m_verticalAngle;
	float m_speed;
	float m_mouseSpeed;

	double m_xpos;
	double m_ypos;

	int m_width;
	int m_height;

	float m_deltaTime;
	float m_lastTime;
	double m_currentTime;
};