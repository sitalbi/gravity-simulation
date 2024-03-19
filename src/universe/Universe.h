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

public:
	Universe();
	~Universe();

	void Update();

	void AddBody(Body* body);

	std::vector<Body*> bodies;
};