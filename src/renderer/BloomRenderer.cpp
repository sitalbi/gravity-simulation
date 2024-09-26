#include "BloomRenderer.h"
#include "Renderer.h"

BloomRenderer::BloomRenderer() : mInit(false) {}

bool BloomRenderer::Init(unsigned int windowWidth, unsigned int windowHeight, Renderer& renderer, unsigned int numMips)
{
    if (mInit) return true;
    mSrcViewportSize = glm::ivec2(windowWidth, windowHeight);
    mSrcViewportSizeFloat = glm::vec2((float)windowWidth, (float)windowHeight);

    mRenderer = &renderer;

    // Bloom FBO
    const unsigned int num_bloom_mips = numMips; 
    bool status = mFBO.Init(windowWidth, windowHeight, num_bloom_mips);
    if (!status) {
        std::cerr << "Failed to initialize bloom FBO - cannot create bloom renderer!\n";
        return false;
    }

    // Shaders
    mDownsampleShader = new Shader("res/shaders/postProcessing/VertexShader.glsl", "res/shaders/postProcessing/DownSamplingFragmentShader.glsl");
    mUpsampleShader = new Shader("res/shaders/postProcessing/VertexShader.glsl", "res/shaders/postProcessing/UpSamplingFragmentShader.glsl");

    // Downsample
    mDownsampleShader->Bind();
    mDownsampleShader->SetUniform1i("srcTexture", 0);
    mDownsampleShader->Unbind();

    // Upsample
    mUpsampleShader->Bind();
    mUpsampleShader->SetUniform1i("srcTexture", 0);
    mUpsampleShader->Unbind();


    mInit = true;
    return true;
}

void BloomRenderer::Destroy()
{
    mFBO.Destroy();
    delete mDownsampleShader;
    delete mUpsampleShader;
    mInit = false;

}

void BloomRenderer::RenderBloomTexture(unsigned int srcTexture, float filterRadius)
{

    mFBO.Bind();
    this->RenderDownsamples(srcTexture);

    this->RenderUpsamples(filterRadius);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Restore viewport
    glViewport(0, 0, mSrcViewportSize.x, mSrcViewportSize.y);
}

unsigned int BloomRenderer::BloomTexture()
{
    return mFBO.MipChain()[0].texture;
}


void BloomRenderer::RenderDownsamples(unsigned int srcTexture)
{
    const std::vector<bloomMip>& mipChain = mFBO.MipChain();

    mDownsampleShader->Bind();
    mDownsampleShader->SetUniform2f("srcResolution", mSrcViewportSizeFloat.x, mSrcViewportSizeFloat.y);

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTexture);

    // Progressively downsample through the mip chain
    for (int i = 0; i < mipChain.size(); i++)
    {
        const bloomMip& mip = mipChain[i];
        glViewport(0, 0, mip.size.x, mip.size.y);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mip.texture, 0);

        mRenderer->DrawQuad();

        mDownsampleShader->SetUniform2f("srcResolution", mip.size.x, mip.size.y);
        glBindTexture(GL_TEXTURE_2D, mip.texture);
    }

    mDownsampleShader->Unbind();
}

void BloomRenderer::RenderUpsamples(float filterRadius)
{
    const std::vector<bloomMip>& mipChain = mFBO.MipChain();

    mUpsampleShader->Bind();
    mUpsampleShader->SetUniform1f("filterRadius", filterRadius);

    // Enable additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    for (int i = mipChain.size() - 1; i > 0; i--)
    {
        const bloomMip& mip = mipChain[i];
        const bloomMip& nextMip = mipChain[i - 1];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip.texture);

        glViewport(0, 0, nextMip.size.x, nextMip.size.y);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, nextMip.texture, 0);

        mRenderer->DrawQuad();
    }

    // Disable additive blending
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); 
    glDisable(GL_BLEND);

    mUpsampleShader->Unbind();
}

BloomRenderer::~BloomRenderer() {
    if (mInit) {
        this->Destroy();
    }
}