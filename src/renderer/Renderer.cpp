#include "Renderer.h"


void Renderer::Draw(Model* model, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const
{
	glm::mat4 mvpMatrix = m_projectionMat * viewMat * modelMat;
	// Set uniforms
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	shader.SetUniformMat4f("u_M", modelMat);
	model->Draw();
}

void Renderer::Draw(Body* body, glm::mat4 viewMat, Shader& shader) const
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), body->GetPosition());
	shader.Bind();
	shader.SetUniform4f("u_Color", body->GetColor().x, body->GetColor().y, body->GetColor().z, body->GetColor().w);
	glm::mat4 mvpMatrix = m_projectionMat * viewMat * modelMat;
	// Set uniforms
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	shader.SetUniformMat4f("u_M", modelMat);
	body->GetModel()->Draw();
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
