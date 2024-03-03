#pragma once

#include "../ECS.h"
#include "../Core/Common.h"
#include "PlatformFactory.h"

namespace Systems {
	
	void Move(ECS& ecs, EntityID id, float deltaTime);
	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window, float deltaTime);
	void ApplyForces(ECS& ecs, EntityID id, float deltaTime);

	void RenderQuad(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);
	void RenderBoxCollider(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);

	// Make sure to handle collisions before moving, as SweptAABB alters velocity for the next move!
	void HandleSolidCollisions(ECS& ecs, EntityID id, float deltaTime);

	void HandlePlayerPassesDeathwall(ECS& ecs, EntityID player, EntityID wall);
	void HandleScreenBounds(ECS& ecs, EntityID player, Camera& camera);
	void MoveCameraWithDeathwall(ECS& ecs, Camera& camera, EntityID wall, float deltaTime);

	void PollRespawnPlatform(ECS& ecs, PlatformFactory& factory, Camera& camera, EntityID platform, EntityID deathwall);

	void RenderAxis(Renderer& renderer, Shader& shader);

}