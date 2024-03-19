#include "Universe.h"

glm::vec3 Universe::CalculateAccelaration(glm::vec3 position, Body* bodyToIgnore)
{
	glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	for (Body* body : bodies)
	{
		if (body != bodyToIgnore)
		{
			float squaredDistance = glm::length2(body->GetPosition() - position);
			glm::vec3 forceDirection = glm::normalize(body->GetPosition() - position);
			acceleration += forceDirection * GravitationalConstant * (body->GetMass()) / squaredDistance;
		}
	}
	return acceleration;
}

Universe::Universe()
{
}

Universe::~Universe()
{
}

void Universe::Update()
{
	// Update velocities first
	for (Body* body : bodies)
	{
		glm::vec3 acceleration = CalculateAccelaration(body->GetPosition(), body);
		body->UpdateVelocity(acceleration);
	}
	// Update positions after all velocities have been updated
	for (Body* body : bodies)
	{
		body->UpdatePosition();
	}
}

void Universe::AddBody(Body* body)
{
	bodies.push_back(body);
}
