#version 330 core

out vec4 color;

in vec4 w_Position;
in vec4 w_Normal;
in vec4 w_LightDirection;

uniform vec4 u_Color;
uniform vec4 u_LightColor;
uniform vec4 u_LightPosition;
uniform float u_LightPower;

void main()
{
    vec4 n = w_Normal;
    vec4 l = w_LightDirection;

    float distance = length(u_LightPosition - w_Position);

    float distanceAttenuation = 1.0 / (distance * distance);
    color = u_Color * u_LightColor * u_LightPower * clamp(dot(n, l), 0, 1) * distanceAttenuation;

}
