#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "GL/glew.h"
#include <iostream>
#include <vector>


class Framebuffer
{
public:
	Framebuffer(int width, int height, int numTextures = 1);
	~Framebuffer();

private:
	unsigned int m_fbo;
	unsigned int m_rbo; // Renderbuffer object

	std::vector<unsigned int> m_textureColorBuffers;

public:

	void Bind();
	void Unbind();

	unsigned int GetTextureColorBuffer(int index) const;
};

