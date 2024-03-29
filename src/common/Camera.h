#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define M_PI 3.14159265358979323846

class Camera
{
public:
	Camera(GLFWwindow* window, glm::mat4 ProjectionMatrix, glm::vec3 Position, glm::vec3 lookAt, int width, int height);
	
	void RotateCamera(glm::vec2 mousePos);

	glm::vec3 GetViewDir() const;
	glm::vec3 GetRightVector() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void SetLookAt(glm::vec3 lookAt);

	void ResetDrag();

	glm::vec3 m_position;
	glm::vec3 m_lookAt;


private:
	GLFWwindow* m_window;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

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

	bool isDragging;

	glm::vec2 lastDragPos;
};