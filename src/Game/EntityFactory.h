#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../ECS.h"
#include "PlatformFactory.h"

EntityID CreatePlayer(ECS& ecs, glm::vec2 position, glm::vec2 size);
EntityID CreatePlatform(ECS& ecs, glm::vec2 position, glm::vec2 size);
EntityID CreateDeathWall(ECS& ecs, float width);
void InitializePlatforms(ECS& ecs, PlatformFactory& factory, float deathwallWidth, std::vector<EntityID>& platforms);