#pragma once
#include "glm/glm.hpp"
#include "Shader.h"
#include "BloomFBO.h"
#include "Renderer.h"

class BloomRenderer
{
public:
    BloomRenderer();
    ~BloomRenderer();
    bool Init(unsigned int windowWidth, unsigned int windowHeight, Renderer& renderer, unsigned int numMips);
    void Destroy();
    void RenderBloomTexture(unsigned int srcTexture, float filterRadius);
    unsigned int BloomTexture();

private:
    void RenderDownsamples(unsigned int srcTexture);
    void RenderUpsamples(float filterRadius);

    bool mInit;
    BloomFBO mFBO;
    glm::ivec2 mSrcViewportSize;
    glm::vec2 mSrcViewportSizeFloat;
    Shader* mDownsampleShader;
    Shader* mUpsampleShader;

    Renderer* mRenderer;
};