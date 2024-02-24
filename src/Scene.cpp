#include "Scene.h"
#include <iostream>
#include <glm/glm.hpp>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include "Components/Component.h"
#include "Game/Systems.h"

EntityID player;
EntityID platform;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_renderer = std::make_unique<Renderer>();
	m_ecs = std::make_unique<ECS>(100);

	ECS& ecs = *m_ecs.get();

	// Register ECS components on scene construction
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Physics>();

	player = ecs.CreateEntity();
	platform = ecs.CreateEntity();

	ecs.AddComponent<Transform>(player);
	ecs.AddComponent<Physics>(player);
	ecs.AddComponent<BoxCollider>(player);
	ecs.AddComponent<Renderable>(player, {glm::vec3(1, 1, 1)});

	ecs.AddComponent<Transform>(platform, {glm::vec2(0, 0), glm::vec2(2.0f, 1.0f)});
	ecs.AddComponent<BoxCollider>(platform, { glm::vec2(0, 0), glm::vec2(2.0f, 1.0f) });
	ecs.AddComponent<Renderable>(platform, { glm::vec3(1, 0, 1) });
}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Renderer& renderer = *m_renderer.get();
	Shader& quadShader = *m_quadShader.get();
	
	Systems::RenderQuad(ecs, platform, renderer, quadShader);
	Systems::RenderQuad(ecs, player, renderer, quadShader);

	Systems::RenderBoxCollider(ecs, platform, renderer, quadShader);
	Systems::RenderBoxCollider(ecs, player, renderer, quadShader);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();

	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow());
	Systems::Move(ecs, player, deltaTime);
	Systems::HandleSolidCollisions(ecs, player);
}