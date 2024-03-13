#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr); 
}

void Renderer::Draw(const Model& model, glm::mat4 projectionMat, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const
{
	glm::mat4 mvpMatrix = projectionMat * viewMat * modelMat;
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	model.Draw();
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
