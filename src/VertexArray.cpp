#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind(); // Bind the vertex array
	vb.Bind(); // Bind the vertex buffer

	const auto& elements = layout.GetElements();
	const VertexBufferElement& element = elements[elements.size()-1];
	glEnableVertexAttribArray(element.attribLocation);
	glVertexAttribPointer(element.attribLocation, element.count, element.type, element.normalized, 0, nullptr);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
