#pragma once
#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	unsigned int m_type;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return m_Count; }
	unsigned int GetType() const { return m_type; }
};