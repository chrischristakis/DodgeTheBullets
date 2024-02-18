#include "Scene.h"
#include <iostream>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include <GLFW/glfw3.h>

#include "Components/Transform.h"

Audio::PlaybackID id;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	//Context::GetAudio()->CreateSound("song", "assets/palmtreepanic.wav");
	//id = Context::GetAudio()->Play("song");

	ECS ecs(10);

	EntityID player = ecs.CreateEntity();
	EntityID monster = ecs.CreateEntity();

	ecs.AddComponent<Transform>(player, new Transform(1, 2));
	ecs.AddComponent<Transform>(monster, new Transform(3, 4));
	Transform& t = ecs.GetComponent<Transform>(player);
	Transform& tm = ecs.GetComponent<Transform>(monster);
	tm.x = 7;

	std::cout << t.x << ", " << t.y << "\n";
}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {
	if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Context::GetWindow()->Close();
}