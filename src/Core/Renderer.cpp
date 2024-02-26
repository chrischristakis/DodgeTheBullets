#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../Context.h"

// ----- HOISTS ------ //
void InitQuadData(unsigned int& vbo, unsigned int& vao, unsigned int& ebo);
void InitLineData(unsigned int& vbo, unsigned int& vao);

// ----- IMPLEMENTATIONS ----- //
Renderer::Renderer() {
	InitQuadData(m_quadVBO, m_quadVAO, m_quadEBO);
	InitLineData(m_lineVBO, m_lineVAO);
}

void Renderer::RenderQuad(Shader& shader, glm::vec2 position, glm::vec2 scale, glm::vec3 color) {

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, -0.1f));
	model = glm::scale(model, glm::vec3(scale, 1.0f));

	glm::mat4 mvp = Context::GetCamera()->GetProjectionMatrix() * model;

	shader.Use();
	shader.SetVec3f("color", color);
	shader.SetMatrix4f("mvp", mvp);
	glBindVertexArray(m_quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	glBindVertexArray(0);
}

void Renderer::RenderQuadOutline(Shader& shader, glm::vec2 position, glm::vec2 scale, glm::vec3 color, float thickness) {

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position, -0.1f));
	model = glm::scale(model, glm::vec3(scale, 1.0f));

	glm::mat4 mvp = Context::GetCamera()->GetProjectionMatrix() * model;

	shader.Use();
	shader.SetVec3f("color", color);
	shader.SetMatrix4f("mvp", mvp);
	glBindVertexArray(m_quadVAO);
	glLineWidth(thickness);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	glBindVertexArray(0);
}

void Renderer::RenderLine(Shader& shader, glm::vec2 start, glm::vec2 end, glm::vec3 color, float thickness) {
	glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);

	// Fill buffer with line data
	float data[] = {
		start.x, start.y,
		end.x, end.y
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.1f));
	glm::mat4 mvp = Context::GetCamera()->GetProjectionMatrix() * model;

	shader.Use();
	shader.SetVec3f("color", color);
	shader.SetMatrix4f("mvp", mvp);

	glBindVertexArray(m_lineVAO);
	glLineWidth(thickness);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Generates and stores quad data into references for VBO and VAO
void InitQuadData(unsigned int& vbo, unsigned int& vao, unsigned int& ebo) {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	float rectVerts[] = {
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
	};

	unsigned char indexList[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectVerts), rectVerts, GL_STATIC_DRAW);
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexList), indexList, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void InitLineData(unsigned int& vbo, unsigned int& vao) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Empty buffer to be during rendering later
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float), 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(vao);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}