#include "EntityFactory.h"
#include "../Components/Component.h"
#include "../Context.h"

EntityID CreatePlayer(ECS& ecs, glm::vec2 position, glm::vec2 size) {
	EntityID player = ecs.CreateEntity();

	ecs.AddComponent<Transform>(player, {position, size });
	ecs.AddComponent<Physics>(player);
	ecs.AddComponent<BoxCollider>(player, { position, size });
	ecs.AddComponent<JumpComponent>(player);
	ecs.AddComponent<Renderable>(player, { glm::vec3(1, 1, 0) });

	return player;
}

EntityID CreatePlatform(ECS& ecs, glm::vec2 position, glm::vec2 size) {
	EntityID platform = ecs.CreateEntity();

	ecs.AddComponent<Transform>(platform, { position, size });
	ecs.AddComponent<BoxCollider>(platform, { position, size });
	ecs.AddComponent<Renderable>(platform, { glm::vec3(1, 1, 1) });

	return platform;
}

EntityID CreateDeathWall(ECS& ecs, float width, float initialSpeed) {
	EntityID deathwall = ecs.CreateEntity();

	float cameraHalfWidth = Context::GetCamera()->GetHalfWidth();
	float cameraHalfHeight = Context::GetCamera()->GetHalfHeight();

	// Aligns to the left edge of the screen, with the height of the screen, and a given width
	ecs.AddComponent<Transform>(deathwall, { { -cameraHalfWidth, -cameraHalfHeight }, { width, cameraHalfHeight * 2 } });
	ecs.AddComponent<Physics>(deathwall, { {initialSpeed, 0.0f} });
	ecs.AddComponent<Renderable>(deathwall, { glm::vec3(1, 0.25f, 0.2f) });

	return deathwall;
}