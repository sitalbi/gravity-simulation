#pragma once

#include "GL/glew.h"
#include "Shader.h"
#include <vector>
#include <string>
#include "stb/stb_image.h"


class Skybox
{
private:
	unsigned int m_vao, m_vbo, m_ebo;
	unsigned int m_textureID;

	void Init();

	unsigned int LoadCubemap(std::vector<std::string> faces);
	
public:
	Skybox(std::vector<std::string> faces);
	~Skybox();

	void Draw(glm::mat4 viewMat, glm::mat4 projectionMat, Shader& shader) const;
};

