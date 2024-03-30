#include "Body.h"

Body::Body(glm::vec3 position, Model& model, glm::vec4 color, glm::vec3 initialVelocity, float mass, float surfaceGravity, float radius)
	: m_position(position), m_model(model), m_color(color), m_mass(mass), m_surfaceGravity(surfaceGravity), m_initialVelocity(initialVelocity), m_radius(radius)
{
	m_velocity = m_initialVelocity;
}

void Body::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Body::SetName(std::string name)
{
	m_name = name;
}

void Body::Translate(glm::vec3 translation)
{
	m_position += translation;
}

void Body::UpdateVelocity(glm::vec3 acceleration)
{
	m_velocity += acceleration;
}

void Body::UpdatePosition()
{
	Translate(m_velocity);
}