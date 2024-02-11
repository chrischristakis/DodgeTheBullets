#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class Renderer {
public:

	Renderer();

	void RenderQuad(Shader& shader, float x, float y, float w, float h, glm::vec3 color = {1, 1, 1});

private:

	unsigned int m_quadVBO, m_quadVAO;

};