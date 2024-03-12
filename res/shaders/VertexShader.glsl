#version 330 core

layout(location = 0) in vec4 position;

out vec4 vColor;

uniform mat4 u_MVP;
uniform vec4 u_Color;

void main()
{
	gl_Position = u_MVP * position;
	vColor = u_Color;
}
