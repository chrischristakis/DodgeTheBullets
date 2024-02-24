#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Renderer {
public:

	Renderer();

	void RenderQuad(Shader& shader, glm::vec2 position, glm::vec2 scale, glm::vec3 color = {1, 1, 1});
	void RenderQuadOutline(Shader& shader, glm::vec2 position, glm::vec2 scale, glm::vec3 color = { 1, 1, 1 }, float thickness = 1.0f);

private:

	unsigned int m_quadVBO, m_quadVAO, m_quadEBO;

};