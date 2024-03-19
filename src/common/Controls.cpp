#include "Controls.h"

Controls::Controls(GLFWwindow* window, glm::mat4 ProjectionMatrix)
{
	this->m_window = window;
	m_position = glm::vec3(0, 0, 5);
	m_horizontalAngle = 3.14f;
	m_verticalAngle = 0.0f;
	m_speed = 3.0f;
	m_mouseSpeed = 0.04f;
	m_xpos = 0;
	m_ypos = 0;
	m_width = 1280;
	m_height = 720;

	m_deltaTime = 0.0f;
	m_lastTime = 0.0f;
	m_currentTime = 0.0f;

	m_ProjectionMatrix = ProjectionMatrix;
}

void Controls::computeMatricesFromInputs()
{
	m_currentTime = glfwGetTime();
	m_deltaTime = float(m_currentTime - m_lastTime);
	m_lastTime = m_currentTime;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwGetCursorPos(m_window, &m_xpos, &m_ypos);
	glfwSetCursorPos(m_window, m_width/ 2, m_height/ 2);

	//Compute new orientation
	m_horizontalAngle += m_mouseSpeed * m_deltaTime * float(m_width / 2 - m_xpos);
	m_verticalAngle += m_mouseSpeed * m_deltaTime * float(m_height / 2 - m_ypos);

	// Limit vertical angle
	m_verticalAngle = fmod(m_verticalAngle, M_PI);

	// Limit horizontal angle
	m_horizontalAngle = fmod(m_horizontalAngle, M_PI * 2);

	//Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	//Right vector
	glm::vec3 right(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	//Up vector
	glm::vec3 up = glm::cross(right, direction);


	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_position += direction * m_deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_position -= direction * m_deltaTime * m_speed;
	}
	// Move right
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_position += right * m_deltaTime * m_speed;
	}
	// Move left
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
		m_position -= right * m_deltaTime * m_speed;
	}
	// Move up
	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_position += up * m_deltaTime * m_speed;
	}
	// Move down
	if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_position -= up * m_deltaTime * m_speed;
	}

	// Camera (View) matrix
	m_ViewMatrix = glm::lookAt(
		m_position,       
		m_position + direction, 
		up 
	);
}

glm::mat4 Controls::getViewMatrix()
{
	return m_ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix()
{
	return m_ProjectionMatrix;
}


