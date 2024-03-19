#include "Body.h"

void Body::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Body::Translate(glm::vec3 translation)
{
	m_position += translation;
}
