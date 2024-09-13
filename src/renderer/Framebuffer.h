#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "GL/glew.h"
#include <iostream>


class Framebuffer
{
public:
	Framebuffer(int width, int height);
	~Framebuffer();

private:
	unsigned int m_fbo;
	unsigned int m_textureColorBuffer;
	unsigned int m_rbo; // Renderbuffer object

public:

	void Bind();
	void Unbind();

	unsigned int GetTextureColorBuffer() const;
};

