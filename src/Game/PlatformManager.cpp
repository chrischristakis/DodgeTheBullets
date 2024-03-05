#include "PlatformManager.h"
#include "../Logging.h"
#include "EntityFactory.h"
#include "../Components/Component.h"
#include "Systems.h"

PlatformManager::PlatformManager(ECS* ecs): m_ecs(ecs) {
	// The platforms spawned in game will follow a fixed sequence,
	// which is initialized here.
	// Also keep track of the width + origin (span) of the sequence so we can resetr platforms
	// to the end of it when they reach the edge
	InitializeSequence();
}

void PlatformManager::InitializeSequence() {
	const float PLATFORM_WIDTH = 2.0f;
	const float PLATFORM_HEIGHT = 0.15f;

	float xOffset = 0.0f;
	float spacing = 9.0f;

	SpawnPlatform({ 0, 0 }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset, 2.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset, -2.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset,  4.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset - PLATFORM_WIDTH / 2.0f,  0.0f }, { PLATFORM_WIDTH * 2.0f, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset, -4.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset,  6.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset - PLATFORM_WIDTH / 2.0f,  2.0f }, { PLATFORM_WIDTH * 2.0f, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset - PLATFORM_WIDTH / 2.0f, -2.0f }, { PLATFORM_WIDTH * 2.0f, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset, -6.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset,  4.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset - PLATFORM_WIDTH / 2.0f,  0.0f }, { PLATFORM_WIDTH * 2.0f, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset, -4.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset, 2.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });
	SpawnPlatform({ xOffset, -2.0f }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;

	SpawnPlatform({ xOffset, 0 }, { PLATFORM_WIDTH, PLATFORM_HEIGHT });

	xOffset += PLATFORM_WIDTH + spacing;
}

void PlatformManager::Reset() {
	ECS& ecs = *m_ecs;
	for (EntityID platform : m_platforms)
		ecs.DeleteEntity(platform);
	m_sequenceWidth = 0.0f;

	InitializeSequence();
}

void PlatformManager::Render(Renderer& renderer, Shader& shader) {
	for (EntityID platform : m_platforms)
		Systems::RenderQuad(*m_ecs, platform, renderer, shader);
}

void PlatformManager::SpawnPlatform(glm::vec2 position, glm::vec2 size) {
	EntityID id = CreatePlatform(*m_ecs, position, size);

	float endX = position.x + size.x;
	if (endX > m_sequenceWidth)
		m_sequenceWidth = endX;

	m_platforms.push_back(id);
}

void PlatformManager::HandleRespawnPlatform(EntityID deathwall) {
	ECS& ecs = *m_ecs;

	Transform& deathwallTransform = ecs.GetComponent<Transform>(deathwall);

	for (EntityID platform : m_platforms) {
		Transform& transform = ecs.GetComponent<Transform>(platform);

		// If platform is behind deathwall, respawn it at the front of the sequence
		if (transform.position.x + transform.scale.x <= deathwallTransform.position.x + deathwallTransform.scale.x) {
			BoxCollider& collider = ecs.GetComponent<BoxCollider>(platform);

			transform.position.x += m_sequenceWidth;
			collider.position.x += m_sequenceWidth; // Don't forget to sync collider
		}

	}
}