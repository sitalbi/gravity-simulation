#include "Renderer.h"


void Renderer::Draw(Model* model, glm::mat4 viewMat, glm::mat4 modelMat, Shader& shader) const
{
	glm::mat4 mvpMatrix = m_projectionMat * viewMat * modelMat;
	// Set uniforms
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	shader.SetUniformMat4f("u_M", modelMat);
	model->Draw();
}

void Renderer::InitQuad()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}



void Renderer::Draw(Body* body, glm::mat4 viewMat, Shader& shader) const
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), body->GetPosition());
	// Scale the model using the radius of the body
	modelMat = glm::scale(modelMat, glm::vec3(body->GetRadius()));

	shader.Bind();
	shader.SetUniform4f("u_Color", body->GetColor().x, body->GetColor().y, body->GetColor().z, body->GetColor().w);
	glm::mat4 mvpMatrix = m_projectionMat * viewMat * modelMat;
	
	shader.SetUniformMat4f("u_MVP", mvpMatrix);
	body->GetModel().Draw();
}


void Renderer::DrawQuad() const
{
	glBindVertexArray(m_quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_quadVAO);
	glDeleteBuffers(1, &m_quadVBO);
}