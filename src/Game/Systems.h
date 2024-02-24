#pragma once

#include "../ECS.h"
#include "../Core/Common.h"

namespace Systems {
	
	void Move(ECS& ecs, EntityID id, float deltaTime);
	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window);

	void RenderQuad(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);
	void RenderBoxCollider(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader);

	void HandleSolidCollisions(ECS& ecs, EntityID id);

}