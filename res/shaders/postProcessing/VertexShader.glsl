#version 330 core

layout(location = 0) in vec4 m_Position;
layout(location = 1) in vec2 m_TexCoords;

out vec2 v_TexCoords;

void main()
{
    gl_Position = m_Position;
    v_TexCoords = m_TexCoords;
}
