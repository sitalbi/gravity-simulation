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
	unsigned int m_quadVAO, m_quadVBO;

	void Draw(Model* model, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const;
	
	void InitQuad();


public: 
	Renderer(glm::mat4 projectionMatrix) : m_projectionMat(projectionMatrix) {
		InitQuad();
	}
	~Renderer();

	void Draw(Body* body, glm::mat4 viewMat, Shader& shader) const;
	void DrawQuad() const;
	void Clear() const;

	unsigned int GetQuadVAO() const { return m_quadVAO; }


};