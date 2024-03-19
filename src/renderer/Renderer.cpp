#include "Renderer.h"


void Renderer::Draw(Model* model, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const
{
	glm::mat4 mvpMatrix = m_projectionMat * viewMat * modelMat;
	shader.Bind();
	// Set Vertex shader uniforms
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	shader.SetUniformMat4f("u_M", modelMat);
	model->Draw();
}

void Renderer::Draw(Body* body, glm::mat4 viewMat, Shader& shader) const
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), body->GetPosition());
	Draw(body->GetModel(), viewMat, modelMat, shader);
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
