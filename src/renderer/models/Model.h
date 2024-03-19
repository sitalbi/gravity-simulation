#pragma once

#include <GL/glew.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Model
{
protected:
	unsigned int m_vertexDataSize;
	unsigned int m_indexDataCount;
	unsigned int m_normalDataSize;
	const float* m_vertexData;
	const unsigned int* m_indexData;
	const float* m_normalData;

	unsigned int m_vaoID;
	unsigned int m_vboID;
	unsigned int m_iboID;
	unsigned int m_nboID;

public:
	Model(const float* m_vertexData, const unsigned int* m_indexData, unsigned int m_vertexDataSize, unsigned int m_indexDataCount, const float* m_normalData, unsigned int m_normalDataSize);
	void init();

	virtual void Draw() const;
};