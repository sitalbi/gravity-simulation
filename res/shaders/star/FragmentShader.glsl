#version 330 core

layout(location = 0) out vec4 BrightColor;
layout(location = 1) out vec4 FragColor;


uniform vec4 u_Color;

void main()
{           
    BrightColor = vec4(u_Color.rgb, 1.0);
    FragColor = vec4(u_Color.rgb, 1.0);
}