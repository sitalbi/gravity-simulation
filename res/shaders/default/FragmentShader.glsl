#version 330 core

out vec4 color;

in vec4 w_Position;
in vec4 Normal;

uniform vec4 u_Color;
uniform vec4 u_LightPosition;
uniform vec4 u_LightColor;

float specularStrength = 0.5;

void main()
{
    vec4 n = normalize(Normal);
    vec4 l = normalize(u_LightPosition - w_Position);  

    float diff = max(dot(n, l), 0.0);
    vec4 diffuseColor = diff * u_LightColor;

    vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1) * u_LightColor;

    vec4 result = (ambientColor + diffuseColor) * u_Color;

    color = result;
}