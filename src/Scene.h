#pragma once

#include <memory>
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"

class Scene {
public:
	Scene();

	void Update(float deltaTime);
	void Render();

private:

	std::unique_ptr<Shader> m_quadShader;
	std::unique_ptr<Renderer> m_Renderer;

};