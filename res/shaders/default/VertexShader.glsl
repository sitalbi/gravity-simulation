#version 330 core

layout(location = 0) in vec4 m_Position;
layout(location = 1) in vec4 m_Normal;

out vec4 w_Position;
out vec4 w_Normal;

uniform mat4 u_MVP;
uniform mat4 u_M;

void main()
{
    // Output position of the vertex : MVP * position
    gl_Position = u_MVP * m_Position;

    // Position of the vertex, in world space : M * position
    w_Position = u_M * m_Position;

    // Normal of the vertex, in world space
    w_Normal = u_M * m_Normal;
}
