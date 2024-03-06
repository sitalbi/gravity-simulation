#include "Controls.hpp"

Controls::Controls(GLFWwindow* window)
{
	this->window = window;
	position = glm::vec3(0, 0, 5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	speed = 3.0f;
	mouseSpeed = 0.015f;
	xpos = 0;
	ypos = 0;
	width = 1280;
	height = 720;

	deltaTime = 0.0f;
	lastTime = 0.0f;
	currentTime = 0.0f;
}

void Controls::computeMatricesFromInputs()
{
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, width/ 2, height/ 2);

	//Compute new orientation
	horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(height / 2 - ypos);

	//Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	//Right vector
	glm::vec3 right(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	//Up vector
	glm::vec3 up = glm::cross(right, direction);


	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Move right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Move left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	// Move up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += up * deltaTime * speed;
	}
	// Move down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		position -= up * deltaTime * speed;
	}

	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera (View) matrix
	ViewMatrix = glm::lookAt(
		position,       
		position + direction, 
		up 
	);
}

glm::mat4 Controls::getViewMatrix()
{
	return ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix()
{
	return ProjectionMatrix;
}


