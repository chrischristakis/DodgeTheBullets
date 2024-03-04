#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../ECS.h"
#include "../Core/Renderer.h"
#include "../Core/Shader.h"

class PlatformManager {
public:

	PlatformManager(ECS* ecs);

	void InitializeSequence();

	void SpawnPlatform(glm::vec2 position, glm::vec2 size);

	void Render(Renderer& renderer, Shader& shader);

	// Respawn platforms when their x position is below the deathwall, move back to the end of the sequence
	void HandleRespawnPlatform(EntityID deathwall);

	void Reset();

private:

	// How much width the initial sequence of platforms occupies
	inline static float m_sequenceWidth = 0.0f;
	
	// non-owning pointer
	ECS* m_ecs;

	std::vector<EntityID> m_platforms;
};