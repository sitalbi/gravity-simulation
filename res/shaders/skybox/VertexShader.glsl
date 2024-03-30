#version 330 core

layout (location = 0) in vec3 m_Position;

out vec3 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TexCoords = m_Position;
    vec4 pos = u_Projection * u_View * vec4(m_Position, 1.0);
    gl_Position = pos.xyww;
}  