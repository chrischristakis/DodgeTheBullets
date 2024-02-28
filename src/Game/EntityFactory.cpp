#include "EntityFactory.h"

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
