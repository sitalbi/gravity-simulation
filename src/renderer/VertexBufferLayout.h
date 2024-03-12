#pragma once

#include "GL/glew.h"
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	unsigned int attribLocation;
	unsigned int offset;

	// Returns the size of the OpenGL type
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
			default: return 0;
		}
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() : m_Stride(0) {}

	void Push(unsigned int type, unsigned int count)
	{
		unsigned int attribLocation = m_Elements.size(); // This is the index of the element in the vector
		unsigned int offset = m_Stride;
		m_Elements.push_back({ type, count, GL_FALSE, attribLocation, offset });
		m_Stride += count * VertexBufferElement::GetSizeOfType(type);
	}

	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }
};