#include "Scene.h"
#include <iostream>

#include "Core/Audio.h"


Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	Audio::CreateSound("song", "assets/palmtreepanic.wav");
	Audio::PlaySound("song", true);
}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {

}