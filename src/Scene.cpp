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

	for (int i = 0; i < 3; i++) {
		EntityID platform = ecs.CreateEntity();
		ecs.AddComponent<Transform>(platform, { glm::vec2(0, i) });
		ecs.AddComponent<BoxCollider>(platform, { glm::vec2(0, i) });
		ecs.AddComponent<Renderable>(platform, { glm::vec3(1, 0, 1) });
		platforms.push_back(platform);
	}

	ecs.AddComponent<Transform>(player);
	ecs.AddComponent<Physics>(player);
	ecs.AddComponent<BoxCollider>(player);
	ecs.AddComponent<Renderable>(player, {glm::vec3(1, 1, 1)});
}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Renderer& renderer = *m_renderer.get();
	Shader& quadShader = *m_quadShader.get();
	
	for (EntityID platform : platforms)
		Systems::RenderQuad(ecs, platform, renderer, quadShader);

	Systems::RenderQuad(ecs, player, renderer, quadShader);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();

	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow());
	Systems::HandleSolidCollisions(ecs, player, deltaTime);
	Systems::Move(ecs, player, deltaTime);
}