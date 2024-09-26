#include "Framebuffer.h"


Framebuffer::Framebuffer(int width, int height, int numTextures)
{
    m_textureColorBuffers.resize(numTextures);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create color attachment textures
    for (int i = 0; i < numTextures; i++) {
        glGenTextures(1, &m_textureColorBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, m_textureColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textureColorBuffers[i], 0);
    }

    // Renderbuffer object
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    // Specify the draw buffers
    std::vector<unsigned int> attachments;
    for (int i = 0; i < numTextures; i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(numTextures, attachments.data());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetTextureColorBuffer(int index) const
{
    if(index >= m_textureColorBuffers.size())
		return 0;
	return m_textureColorBuffers[index];
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(m_textureColorBuffers.size(), m_textureColorBuffers.data());
    glDeleteRenderbuffers(1, &m_rbo);
}
