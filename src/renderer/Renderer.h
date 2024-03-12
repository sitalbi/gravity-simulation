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
	void Draw(const Model& model, glm::mat4 mvpMatrix ,Shader& shader) const;
	void Clear() const;
};