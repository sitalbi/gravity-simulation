#include "Camera.h"

Camera::Camera(GLFWwindow* window, glm::mat4 ProjectionMatrix, glm::vec3 Position, glm::vec3 lookAt, int width, int height)
{
	this->m_window = window;
	m_position = Position;
	m_lookAt = lookAt;
	m_horizontalAngle = 3.14f;
	m_verticalAngle = 0.0f;
	m_speed = 6.0f;
	m_mouseSpeed = 0.04f;
	m_xpos = 0;
	m_ypos = 0;
	m_width = width;
	m_height = height;

	m_deltaTime = 0.0f;
	m_lastTime = 0.0f;
	m_currentTime = 0.0f;

	m_ProjectionMatrix = ProjectionMatrix;
	m_ViewMatrix = glm::lookAt(
		m_position,       
		m_lookAt, 
		glm::vec3(0, 1, 0)  
	);
}

/*
* Rotate the camera based on the mouse position (arcball camera)
*/
void Camera::RotateCamera(glm::vec2 mousePos)
{
	glm::vec3 upVector = glm::vec3(0, 1, 0);

	glm::vec4 pivot = glm::vec4(m_lookAt, 1.0f);
	glm::vec4 position = glm::vec4(m_position, 1.0f);

	if (!isDragging) {
		isDragging = true;
		lastDragPos = mousePos;
	}

	// Calculate the amount of rotation given the mouse movement.
	float deltaAngleX = (glm::two_pi<float>() / m_width);
	float deltaAngleY = (glm::two_pi<float>() / m_height);
	float xAngle = (float)(lastDragPos.x - mousePos.x) * deltaAngleX;
	float yAngle = (float)(lastDragPos.y - mousePos.y) * deltaAngleY;

	// Extra step to handle the problem when the camera direction is the same as the up vector
	float cosAngle = glm::dot(GetViewDir(), upVector);
	if (cosAngle * glm::sign(yAngle) > 0.99f) {
		yAngle = 0;
	}

	// Rotation on first axis
	glm::mat4 rotationMatrixX(1.0f);
	rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, upVector);
	position = (rotationMatrixX * (position - pivot)) + pivot;

	// Rotation on second axis
	glm::mat4 rotationMatrixY(1.0f);
	rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightVector());
	m_position = (rotationMatrixY * (position - pivot)) + pivot;

	lastDragPos = mousePos;
}

glm::vec3 Camera::GetViewDir() const
{
	return -glm::transpose(m_ViewMatrix)[2]; 
}

glm::vec3 Camera::GetRightVector() const
{
	return glm::transpose(m_ViewMatrix)[0];
}

void Camera::ResetDrag()
{
	isDragging = false;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(
		m_position,       
		m_lookAt, 
		glm::vec3(0, 1, 0)  
	);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void Camera::SetLookAt(glm::vec3 lookAt)
{
	m_lookAt = lookAt;
}


