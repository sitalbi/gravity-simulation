#version 330 core

// Upsampling on a texture, as taken from Call Of Duty method, presented at ACM Siggraph 2014.

uniform sampler2D srcTexture;
uniform float filterRadius;

in vec2 TexCoords;
layout (location = 0) out vec4 upsample;

void main()
{
    float x = filterRadius;
    float y = filterRadius;

    vec3 a = texture(srcTexture, vec2(TexCoords.x - x, TexCoords.y + y)).rgb;
    vec3 b = texture(srcTexture, vec2(TexCoords.x,     TexCoords.y + y)).rgb;
    vec3 c = texture(srcTexture, vec2(TexCoords.x + x, TexCoords.y + y)).rgb;

    vec3 d = texture(srcTexture, vec2(TexCoords.x - x, TexCoords.y)).rgb;
    vec3 e = texture(srcTexture, vec2(TexCoords.x,     TexCoords.y)).rgb;
    vec3 f = texture(srcTexture, vec2(TexCoords.x + x, TexCoords.y)).rgb;

    vec3 g = texture(srcTexture, vec2(TexCoords.x - x, TexCoords.y - y)).rgb;
    vec3 h = texture(srcTexture, vec2(TexCoords.x,     TexCoords.y - y)).rgb;
    vec3 i = texture(srcTexture, vec2(TexCoords.x + x, TexCoords.y - y)).rgb;

    vec3 color = e*4.0;
    color += (b+d+f+h)*2.0;
    color += (a+c+g+i);
    color *= 1.0 / 16.0;

    upsample = vec4(color, 1.0);
}