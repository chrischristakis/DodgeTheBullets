#include "Scene.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Context.h"
#include "ECS.h"
#include "Game/Systems.h"
#include "Game/EntityFactory.h"

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

	player = CreatePlayer(ecs, { 0, 0 }, { 1, 1 });
	platforms.push_back(CreatePlatform(ecs, {0, 1}, {1, 1}));
	platforms.push_back(CreatePlatform(ecs, {2, 1}, {2, 1}));
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

	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow(), deltaTime);
	Systems::ApplyForces(ecs, player, deltaTime);
	Systems::HandleSolidCollisions(ecs, player, deltaTime);
	Systems::Move(ecs, player, deltaTime);

}