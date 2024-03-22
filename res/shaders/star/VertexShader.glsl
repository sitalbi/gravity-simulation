#version 330 core

layout(location = 0) in vec4 m_Position;
layout(location = 1) in vec4 m_Normal;

uniform mat4 u_MVP;
uniform mat4 u_M;

void main()
{
    // Output position of the vertex : MVP * position
    gl_Position = u_MVP * m_Position;
}
