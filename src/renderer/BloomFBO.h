#pragma once
#include "glm/glm.hpp"
#include <vector>

struct bloomMip
{
    glm::vec2 size;
    glm::ivec2 intSize;
    unsigned int texture;
};

class BloomFBO
{
    public:
	BloomFBO();
	~BloomFBO();

    bool Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength);
    void Destroy();
    void Bind();
    const std::vector<bloomMip>& MipChain() const;

private:
    bool mInit;
    unsigned int mFBO;
    std::vector<bloomMip> mMipChain;
};

