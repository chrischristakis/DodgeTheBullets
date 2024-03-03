#include "EntityFactory.h"
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

EntityID CreateDeathWall(ECS& ecs, float width) {
	EntityID deathwall = ecs.CreateEntity();

	float cameraHalfWidth = Context::GetCamera()->GetHalfWidth();
	float cameraHalfHeight = Context::GetCamera()->GetHalfHeight();

	// Aligns to the left edge of the screen, with the height of the screen, and a given width
	ecs.AddComponent<Transform>(deathwall, { { -cameraHalfWidth, -cameraHalfHeight }, { width, cameraHalfHeight * 2 } });
	ecs.AddComponent<Physics>(deathwall, { {3.0f, 0.0f} });
	ecs.AddComponent<Renderable>(deathwall, { glm::vec3(1, 0.25f, 0.2f) });

	return deathwall;
}

void InitializePlatforms(ECS& ecs, PlatformFactory& factory, float deathwallWidth, std::vector<EntityID>& platforms) {
	glm::vec2 platformSize = factory.GetPlatformSize();
	float cameraHalfWidth = Context::GetCamera()->GetHalfWidth();

	// Spawn segments with enough spacing so that when the death wall hits the first platform/segment,
	// it will respawn right beside the last segment.
	// Assuming first segment spawns in middle of the screen, this formula is (camerahalfWidth*2 - deathwallWidth) / platformWidth
	// The + platformwidth at the end is since the platforms despawn once their entire width goes into the deathwall.
	int numOfSegments = ((cameraHalfWidth * 2) - deathwallWidth + platformSize.x) / platformSize.x;

	for (int i = 0; i < numOfSegments; i++) {
		Segment segment = factory.GetNextSegmentInSequence();

		std::cout << "Segment " << i << std::endl;
		for (Platform& platform : segment) {
			float xOffset = i * platformSize.x;
			std::cout << platform.position.x << ", " << platform.position.y << std::endl;
			platforms.push_back(CreatePlatform(ecs, { xOffset, platform.position.y }, { platformSize }));
		}
	}
}