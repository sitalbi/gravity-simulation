#pragma once

#include "glm/glm.hpp"
#include "../renderer/models/Model.h"
#include "../renderer/Shader.h"

class Body
{
private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_initialVelocity;
	float m_mass;
	float m_surfaceGravity;

	glm::vec4 m_color;

	Model* m_model;

public:
	Body(glm::vec3 position, Model* model, glm::vec4 color, glm::vec3 initialVelocity, float mass, float surfaceGravity);

	void SetPosition(glm::vec3 position);
	void Translate(glm::vec3 translation);
	void UpdateVelocity(glm::vec3 acceleration);
	void UpdatePosition();

	glm::vec3 GetPosition() const { return m_position; }
	Model* GetModel() const { return m_model; }
	glm::vec4 GetColor() const { return m_color; }
	float GetMass() const { return m_mass; }
};