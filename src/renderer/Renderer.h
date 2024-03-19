#pragma once

#include "GL/glew.h"

#include "Shader.h"
#include "models/Model.h"
#include "glm/glm.hpp"
#include "../universe/Body.h"

class Renderer
{
private:
	glm::mat4 m_projectionMat;

	void Draw(Model* model, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const;

public: 
	Renderer(glm::mat4 projectionMatrix) : m_projectionMat(projectionMatrix) {}

	void Draw(Body* body, glm::mat4 viewMat, Shader& shader) const;
	void Clear() const;
};