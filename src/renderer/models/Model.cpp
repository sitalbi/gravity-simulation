#include "Model.h"

Model::Model(const float* vertexData, const unsigned int* indexData, unsigned int vertexDataSize, unsigned int indexDataCount, const float* m_normalData, unsigned int m_normalDataSize)
	: m_vertexDataSize(vertexDataSize), m_indexDataCount(indexDataCount), m_vertexData(vertexData), m_indexData(indexData), m_normalData(m_normalData), m_normalDataSize(m_normalDataSize)
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

    // Create and bind normal buffer
    glGenBuffers(1, &m_nboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_nboID);
    glBufferData(GL_ARRAY_BUFFER, m_normalDataSize, m_normalData, GL_STATIC_DRAW);

    // Normal buffer layout
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_nboID);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Model::Draw() const
{
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
	glDrawElements(GL_TRIANGLES, m_indexDataCount, GL_UNSIGNED_INT, nullptr);
}

