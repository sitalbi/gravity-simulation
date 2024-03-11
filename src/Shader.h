#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_vertexFilePath;
	std::string m_fragmentFilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache; // Cache for uniforms

	unsigned int CompileShader();
	int GetUniformLocation(const std::string& name);

public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};