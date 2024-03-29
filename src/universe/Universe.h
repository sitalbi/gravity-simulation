#pragma once

#include "Body.h"
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <vector>

#define GravitationalConstant 0.0001f

class Universe
{
private:
	glm::vec3 CalculateAccelaration(glm::vec3 position, Body* bodyToIgnore);

	unsigned int m_emmisiveBodyId;
	unsigned int m_focusedBodyId;

public:
	Universe();
	~Universe();

	void Update();

	void AddBody(Body* body);

	std::vector<Body*> bodies;

	void SetEmissiveBody(Body* body);
	void SetFocusedBody(unsigned int bodyId);

	unsigned int GetEmissiveBodyId() const;
	unsigned int GetFocusedBodyId() const;

};