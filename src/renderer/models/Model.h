#pragma once

#include <GL/glew.h>
#include <vector>

#include "renderer/VertexArray.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/VertexBufferLayout.h"

class Model
{
protected:
	unsigned int m_vertexDataSize;
	unsigned int m_indexDataCount;
	const float* m_vertexData;
	const unsigned int* m_indexData;

	unsigned int m_vaoID;
	unsigned int m_vboID;
	unsigned int m_iboID;
	unsigned int m_layoutID;

public:
	Model(const float* m_vertexData, const unsigned int* m_indexData, unsigned int m_vertexDataSize, unsigned int m_indexDataCount);
	void init();

	virtual void Draw() const;
};