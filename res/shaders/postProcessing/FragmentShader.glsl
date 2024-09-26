#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform float bloomExposure;

void main()
{             
    const float gamma = 2.2;
    
    // Fetch the scene and bloom color
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    // Only apply uniform exposure to the bloom scene
    vec3 sceneColor = vec3(1.0) - exp(-hdrColor);
    vec3 bloomColorAdjusted = vec3(1.0) - exp(-bloomColor * bloomExposure);

    // Additive blending
    vec3 result = sceneColor + bloomColorAdjusted;

    // Gamma correction
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}
