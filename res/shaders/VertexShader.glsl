#version 330 core

layout(location = 0) in vec4 m_Position;
layout(location = 1) in vec4 m_Normal;

out vec4 w_Position;
out vec4 w_Normal;
out vec4 w_LightDirection;

uniform mat4 u_MVP;
uniform mat4 u_M;
uniform vec4 u_LightPosition;

void main()
{
    // Output position of the vertex : MVP * position
    gl_Position = u_MVP * m_Position;

    // Position of the vertex, in world space : M * position
    w_Position = u_M * m_Position;

    // Vector that goes from the vertex to the light, in camera space
    w_LightDirection = normalize(u_LightPosition - w_Position);

    // Normal of the vertex, in world space
    w_Normal = u_M * m_Normal;
}
