#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;


in vec2 v_TexCoords;

uniform sampler2D screenTexture;


void main()
{
	vec3 col = texture(screenTexture, v_TexCoords).rgb;

    FragColor = texture(screenTexture, v_TexCoords);

    float brightness = dot(col, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
		BrightColor = vec4(col, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}  