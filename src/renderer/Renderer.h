#pragma once

#include "GL/glew.h"

#include "Shader.h"
#include "models/Model.h"
#include "glm/glm.hpp"

class Renderer
{
private:


public: 
	void Draw(Model& model, glm::mat4 projectionMat, glm::mat4 viewMat, glm::mat4 modelMat,Shader& shader) const;
	void Clear() const;
};