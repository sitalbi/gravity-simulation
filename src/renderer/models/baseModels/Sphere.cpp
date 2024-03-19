#include "Sphere.h"
#include <cmath>

// Initialize the Sphere data here
std::vector<float> vertices;
std::vector<unsigned int> indices;
std::vector<float> normals;


Sphere::Sphere(float radius, unsigned int segments)
    : Model(nullptr, nullptr, 0, 0, nullptr, 0)
{
    generateSphere(radius, segments);
    init();
}

void Sphere::generateSphere(float radius, unsigned int segments) {
    const float pi = glm::pi<float>();
    const float pi2 = pi * 2.0f;

    for (unsigned int y = 0; y <= segments; ++y) {
        for (unsigned int x = 0; x <= segments; ++x) {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            float xPos = std::cos(xSegment * pi2) * std::sin(ySegment * pi);
            float yPos = std::cos(ySegment * pi);
            float zPos = std::sin(xSegment * pi2) * std::sin(ySegment * pi);

            vertices.push_back(xPos * radius);
            vertices.push_back(yPos * radius);
            vertices.push_back(zPos * radius);

            normals.push_back(xPos);
            normals.push_back(yPos);
            normals.push_back(zPos);
        }
    }

    for (unsigned int y = 0; y < segments; ++y) {
        for (unsigned int x = 0; x < segments; ++x) {
            indices.push_back((y + 1) * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x + 1);

            indices.push_back((y + 1) * (segments + 1) + x);
            indices.push_back(y * (segments + 1) + x + 1);
            indices.push_back((y + 1) * (segments + 1) + x + 1);
        }
    }

    m_vertexData = vertices.data();
    m_indexData = indices.data();
    m_normalData = normals.data();
    m_vertexDataSize = vertices.size() * sizeof(float);
    m_indexDataCount = indices.size();
    m_normalDataSize = normals.size() * sizeof(float);
}

void Sphere::Draw() const {
    Model::Draw();
}
