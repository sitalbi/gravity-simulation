#include "Cube.h"

// Cube vertices
static const std::vector<float> g_VertexBufferData = {
    // Front face
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,

    // Back face
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,

     // Right face
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

     // Left face
     -0.5f, -0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,

     // Top face
     -0.5f, 0.5f,  0.5f,
      0.5f, 0.5f,  0.5f,
      0.5f, 0.5f, -0.5f,
     -0.5f, 0.5f, -0.5f,

     // Bottom face
     -0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f
};

// Indices data
static const std::vector<unsigned int> g_IndexBufferData = {
        0, 1, 2, 0, 2, 3,       // Front face
        4, 5, 6, 4, 6, 7,       // Back face
        8, 9, 10, 8, 10, 11,    // Right face
        12, 13, 14, 12, 14, 15, // Left face
        16, 17, 18, 16, 18, 19, // Top face
        20, 21, 22, 20, 22, 23  // Bottom face
};

Cube::Cube() : Model(g_VertexBufferData.data(), g_IndexBufferData.data(), g_VertexBufferData.size() * sizeof(float), g_IndexBufferData.size())
{
    init();
}

void Cube::Draw() const {
	Model::Draw();
}
