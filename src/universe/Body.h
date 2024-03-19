#pragma once

#include "glm/glm.hpp"
#include "../renderer/models/Model.h"
#include "../renderer/Shader.h"

class Body
{
private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;

	glm::vec4 m_color;

	Model* m_model;

public:
	Body(glm::vec3 position, Model* model)
		: m_position(position), m_model(model) {}

	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);

	glm::vec3 GetPosition() const { return m_position; }
	Model* GetModel() const { return m_model; }
};