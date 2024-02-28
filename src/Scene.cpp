#include "Scene.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include "Components/Component.h"
#include "Game/Systems.h"

#include "Game/CollisionUtil.h"

EntityID player;
std::vector<EntityID> platforms;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_renderer = std::make_unique<Renderer>();
	m_ecs = std::make_unique<ECS>(100);

	ECS& ecs = *m_ecs.get();

	// Register ECS components on scene construction
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Physics>();

	player = ecs.CreateEntity();

	EntityID platform = ecs.CreateEntity();
	ecs.AddComponent<Transform>(platform, { glm::vec2(0, 2), glm::vec2(3, 1) });
	ecs.AddComponent<BoxCollider>(platform, { glm::vec2(0, 2), glm::vec2(3, 1) });
	ecs.AddComponent<Renderable>(platform, { glm::vec3(1, 1, 1) });
	platforms.push_back(platform);

	platform = ecs.CreateEntity();
	ecs.AddComponent<Transform>(platform, { glm::vec2(-9, -1), glm::vec2(6.5f, 0.1f) });
	ecs.AddComponent<BoxCollider>(platform, { glm::vec2(-9, -1), glm::vec2(6.5f, 0.1f) });
	ecs.AddComponent<Renderable>(platform, { glm::vec3(1, 1, 1) });
	platforms.push_back(platform);

	ecs.AddComponent<Transform>(player);
	ecs.AddComponent<Physics>(player, {{0, 0}, {50.0f, 50.0f}, {0.1f, 1.0f}});
	ecs.AddComponent<BoxCollider>(player);
	ecs.AddComponent<Renderable>(player, {glm::vec3(1, 1, 0)});
}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Renderer& renderer = *m_renderer.get();
	Shader& quadShader = *m_quadShader.get();
	
	for (EntityID platform : platforms)
		Systems::RenderQuad(ecs, platform, renderer, quadShader);

	Systems::RenderQuad(ecs, player, renderer, quadShader);

	Systems::RenderAxis(renderer, quadShader);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();

	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow(), deltaTime);
	Systems::ApplyPhysics(ecs, player, deltaTime);
	Systems::HandleSolidCollisions(ecs, player, deltaTime);
	Systems::Move(ecs, player, deltaTime);
}