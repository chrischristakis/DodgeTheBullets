#include "Scene.h"
#include <iostream>

#include "Core/Audio.h"
#include "Context.h"
#include "ECS.h"
#include <GLFW/glfw3.h>

#include "Components/Transform.h"
#include "Components/Physics.h"

#include <chrono>

Audio::PlaybackID id;

void ProfileECS(int COUNT) {
	std::vector<EntityID> entities;
	ECS ecs(COUNT);
	auto start = std::chrono::high_resolution_clock::now();
	{
		for (int i = 0; i < COUNT; i++) {
			EntityID id = ecs.CreateEntity();
			ecs.AddComponent<Transform>(id, Transform(1, 2));
			ecs.AddComponent<Physics>(id, Physics(1, 2));
			entities.push_back(id);
		}

		for (int i = 0; i < COUNT; i++) {
			Transform& t = ecs.GetComponent<Transform>(entities[i]);
			Physics& p = ecs.GetComponent<Physics>(entities[i]);
		}

		for (int i = 0; i < COUNT; i++) {
			ecs.DeleteComponent<Transform>(entities[i]);
			ecs.DeleteComponent<Physics>(entities[i]);
		}

	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time ECS: " << duration.count() << " seconds." << std::endl;
}

void ProfileComposition(int COUNT) {
	std::vector<Entity> entities;

	for (int i = 0; i < COUNT; i++) {
		entities.push_back(Entity());
	}

	auto start = std::chrono::high_resolution_clock::now();
	{
		for (int i = 0; i < COUNT; i++) {
			Entity& e = entities[i];
			e.AddComponent<Transform>(Transform(1, 2));
			e.AddComponent<Physics>(Physics(1, 2));
		}

		for (int i = 0; i < COUNT; i++) {
			Entity& e = entities[i];
			Transform& t = e.GetComponent<Transform>();
			Physics& p = e.GetComponent<Physics>();
		}

		for (int i = 0; i < COUNT; i++) {
			Entity& e = entities[i];
			e.DeleteComponent<Transform>();
			e.DeleteComponent<Physics>();
		}

	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time COMPOSITION: " << duration.count() << " seconds." << std::endl;
}

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	//Context::GetAudio()->CreateSound("song", "assets/palmtreepanic.wav");
	//id = Context::GetAudio()->Play("song");


	const int COUNT = 1000000;
	ProfileComposition(COUNT);
	ProfileECS(COUNT);

}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {
	if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Context::GetWindow()->Close();
}