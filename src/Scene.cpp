#include "Scene.h"
#include <iostream>

#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

Scene::Scene() {
	m_quadShader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	m_Renderer = std::make_unique<Renderer>();

	FMOD::System* system;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD_RESULT result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		std::cerr << "Failed to create FMOD system: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	result = system->init(32, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		std::cerr << "Failed to initialize FMOD system: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	result = system->createSound("assets/palmtreepanic.wav", FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK) {
		std::cerr << "Failed to load FMOD sound: " << FMOD_ErrorString(result) << std::endl;
		return;
	}

	sound->setMode(FMOD_LOOP_NORMAL); // Loop sound

	result = system->playSound(sound, 0, false, &channel);
}

void Scene::Render() {
	m_Renderer->RenderQuad(m_quadShader.get(), 0, 0, 1.0f, 1.0f);
}

void Scene::Update(float deltaTime) {
}