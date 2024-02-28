#pragma once

#include "../ECS.h"
#include "../Core/Common.h"

namespace Systems {
	
	void Move(ECS& ecs, EntityID id, float deltaTime);
	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window, float deltaTime);
	void ApplyForces(ECS& ecs, EntityID id, float deltaTime);

	void RenderQuad(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);
	void RenderBoxCollider(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);

	// Make sure to handle collisions before moving, as SweptAABB alters velocity for the next move!
	void HandleSolidCollisions(ECS& ecs, EntityID id, float deltaTime);

	void RenderAxis(Renderer& renderer, Shader& shader);

}