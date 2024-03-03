#include "Scene.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Context.h"
#include "ECS.h"
#include "Game/Systems.h"
#include "Game/EntityFactory.h"

EntityID player, deathwall;
std::vector<EntityID> platforms;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_renderer = std::make_unique<Renderer>();
	m_ecs = std::make_unique<ECS>(100);
	m_platformFactory = std::make_unique<PlatformFactory>(3.0f, 0.25f);

	ECS& ecs = *m_ecs.get();

	// Register ECS components on scene construction
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Physics>();

	float deathWallWidth = 5.0f;

	player = CreatePlayer(ecs, { 0, -5 }, { 1, 1 });
	deathwall = CreateDeathWall(ecs, deathWallWidth);

	InitializePlatforms(ecs, *m_platformFactory.get(), deathWallWidth, platforms);
}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Renderer& renderer = *m_renderer.get();
	Shader& quadShader = *m_quadShader.get();
	
	for (EntityID platform : platforms)
		Systems::RenderQuad(ecs, platform, renderer, quadShader);

	Systems::RenderQuad(ecs, player, renderer, quadShader);
	Systems::RenderQuad(ecs, deathwall, renderer, quadShader);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();

	// PLAYER
	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow(), deltaTime);
	Systems::ApplyForces(ecs, player, deltaTime);
	Systems::HandleSolidCollisions(ecs, player, deltaTime);
	Systems::Move(ecs, player, deltaTime);
	Systems::HandleScreenBounds(ecs, player, *Context::GetCamera());

	// DEATH WALL
	Systems::Move(ecs, deathwall, deltaTime);
	Systems::HandlePlayerPassesDeathwall(ecs, player, deathwall);
	Systems::MoveCameraWithDeathwall(ecs, *Context::GetCamera(), deathwall, deltaTime);

	for (EntityID platform : platforms) {
		Systems::PollRespawnPlatform(ecs, *Context::GetCamera(), platform, deathwall);
	}
}