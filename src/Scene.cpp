#include "Scene.h"
#include <iostream>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include <GLFW/glfw3.h>

#include "Components/Transform.h"
#include "Components/Physics.h"

Audio::PlaybackID id;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	//Context::GetAudio()->CreateSound("song", "assets/palmtreepanic.wav");
	//id = Context::GetAudio()->Play("song");

	ECS ecs(5);

	EntityID player = ecs.CreateEntity();
	EntityID monster = ecs.CreateEntity();

	Transform& t = ecs.AddComponent<Transform>(player, { 10, 10, 10, 10 });
	ecs.AddComponent<Transform>(monster, { 10, 10 });
	ecs.AddComponent<Physics>(monster, { 10, 20 });

}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {
	if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Context::GetWindow()->Close();
}