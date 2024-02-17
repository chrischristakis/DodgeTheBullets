#include "Scene.h"
#include <iostream>

#include "Core/Audio.h"
#include "Context.h"
#include <GLFW/glfw3.h>

Audio::PlaybackID id;

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	//Context::GetAudio()->CreateSound("song", "assets/palmtreepanic.wav");
	//id = Context::GetAudio()->Play("song");
}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 10.0f, 10.0f, {0.7f, 0.2f, 0.1f});
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {
	if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Context::GetWindow()->Close();
}