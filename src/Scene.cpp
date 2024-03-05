#include "Scene.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Context.h"
#include "ECS.h"
#include "Components/Component.h"
#include "Game/Systems.h"
#include "Game/EntityFactory.h"

EntityID player, deathwall;
std::vector<EntityID> platforms;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_renderer = std::make_unique<Renderer>();
	m_ecs = std::make_unique<ECS>(100);

	ECS& ecs = *m_ecs.get();

	// Register ECS components on scene construction
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Physics>();
	  
	float deathWallWidth = 5.0f;

	player = CreatePlayer(ecs, { 0, -5 }, { 1, 1 });
	deathwall = CreateDeathWall(ecs, deathWallWidth, 4.0f);

	// Platform manager init
	m_platformManager = std::make_unique<PlatformManager>(m_ecs.get());
}

void Scene::Reset() {
	m_platformManager.get()->Reset();


}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Renderer& renderer = *m_renderer.get();
	Shader& quadShader = *m_quadShader.get();
	PlatformManager& platformManager = *m_platformManager.get();
	
	platformManager.Render(renderer, quadShader);

	Systems::RenderQuad(ecs, player, renderer, quadShader);
	Systems::RenderQuad(ecs, deathwall, renderer, quadShader);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();
	PlatformManager& platformManager = *m_platformManager.get();

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

	platformManager.HandleRespawnPlatform(deathwall);

}