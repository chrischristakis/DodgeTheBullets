#pragma once

#include "../ECS.h"
#include "../Core/Window.h"

namespace Systems {
	
	void Move(ECS& ecs, EntityID id, float deltaTime);
	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window);

}