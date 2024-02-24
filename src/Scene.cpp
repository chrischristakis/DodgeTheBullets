#include "Scene.h"
#include <iostream>
#include <glm/glm.hpp>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include "Components/Common.h"

#include "Game/Systems.h"

EntityID player;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();
	m_ecs = std::make_unique<ECS>(100);

	ECS& ecs = *m_ecs.get();

	// Register ECS components on scene construction
	ecs.RegisterComponent<Transform>();
	ecs.RegisterComponent<Physics>();

	player = ecs.CreateEntity();

	ecs.AddComponent<Transform>(player);
	ecs.AddComponent<Physics>(player);

}

void Scene::Render() {
	ECS& ecs = *m_ecs.get();
	Transform& transform = ecs.GetComponent<Transform>(player);

	m_Renderer->RenderQuad(m_quadShader.get(), transform.position.x, transform.position.y, transform.scale.x, transform.scale.y);
}

void Scene::Update(float deltaTime) {
	ECS& ecs = *m_ecs.get();

	Systems::ProcessMovementInput(ecs, player, *Context::GetWindow());
	Systems::Move(ecs, player, deltaTime);
}