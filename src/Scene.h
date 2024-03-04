#pragma once

#include <memory>
#include "ECS.h"
#include "Core/Renderer.h"
#include "Core/Shader.h"
#include "Game/PlatformManager.h"

class Scene {
public:
	Scene();

	void Update(float deltaTime);
	void Render();

private:

	std::unique_ptr<Shader> m_quadShader;
	std::unique_ptr<Renderer> m_renderer;
	std::unique_ptr<PlatformManager> m_platformManager;
	std::unique_ptr<ECS> m_ecs;

};