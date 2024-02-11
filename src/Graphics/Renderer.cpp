#include "Renderer.h"

// ----- HOISTS ------ //
void InitQuadData(unsigned int& vbo, unsigned int& vao);

// ----- IMPLEMENTATIONS ----- //
Renderer::Renderer() {
	InitQuadData(m_quadVBO, m_quadVAO);
}

void Renderer::RenderQuad(Shader& shader, float x, float y, float w, float h, glm::vec3 color) {
	glBindVertexArray(m_quadVAO);

	glm::mat4 mvp(1.0f);

	shader.Use();
	shader.SetVec3f("color", color);
	shader.SetMatrix4f("mvp", mvp);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

// Generates and stores quad data into references for VBO and VAO
void InitQuadData(unsigned int& vbo, unsigned int& vao) {
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	float data[] = {
		// x     y
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
		-0.5f, -0.5f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}