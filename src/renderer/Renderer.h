#pragma once

#include "GL/glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "models/Model.h"
#include "glm/glm.hpp"

class Renderer
{
private:


public: 
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const Model& model, glm::mat4 projectionMat, glm::mat4 viewMat, glm::mat4 modelMat,Shader& shader) const;
	void Clear() const;
};