#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_Skybox;

void main()
{    
    vec4 color = texture(u_Skybox, TexCoords);
    FragColor = vec4(color.rgb, 1.0);
}