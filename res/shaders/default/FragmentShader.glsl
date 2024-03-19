#version 330 core

out vec4 color;

in vec4 w_Position;
in vec4 w_Normal;

uniform vec4 u_Color;
uniform vec4 u_LightPosition; // Position of the point light
uniform vec4 u_LightColor;
uniform float u_LightPower;

void main()
{
    vec4 n = normalize(w_Normal);
    vec4 lightDirection = normalize(u_LightPosition - w_Position); // Calculate light direction from fragment to light

    float distanceToLight = length(u_LightPosition - w_Position); // Calculate distance to light

    vec4 ambientColor = vec4(0.15, 0.15, 0.15, 1) * u_Color;

    // Attenuation based on distance (inverse square law)
    float attenuation = 1.0 / (distanceToLight * distanceToLight);

    // Diffuse lighting
    vec4 diffuseColor = u_Color * u_LightColor * u_LightPower * clamp(dot(n, lightDirection), 0, 1);

    // Final color with attenuation
    color = ambientColor + diffuseColor;
}

