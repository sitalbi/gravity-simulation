#include "Model.h"

Model::Model(const float* vertexData, const unsigned int* indexData, unsigned int vertexDataSize, unsigned int indexDataCount)
	: m_vertexDataSize(vertexDataSize), m_indexDataCount(indexDataCount), m_vertexData(vertexData), m_indexData(indexData)
{
}


void Model::init() {
    // Create vertex array
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

    
    // Create and bind vertex buffer
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, m_vertexDataSize, m_vertexData, GL_STATIC_DRAW);

    // Vertex buffer layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Create and bind index buffer
    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexDataCount * sizeof(unsigned int), m_indexData, GL_STATIC_DRAW);
}

void Model::Draw() const
{
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glDrawElements(GL_TRIANGLES, m_indexDataCount, GL_UNSIGNED_INT, nullptr);
}

