#pragma once

#include "../Model.h"

class Cube : public Model
{
public:
	Cube();

	void Draw() const override;
};