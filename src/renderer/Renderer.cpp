#include "Renderer.h"


void Renderer::Draw(Model& model, glm::mat4 projectionMat, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const
{
	glm::mat4 mvpMatrix = projectionMat * viewMat * modelMat;
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	shader.SetUniformMat4f("u_M", modelMat);
	model.Draw();
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
