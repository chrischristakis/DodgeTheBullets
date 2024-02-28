#pragma once

#include <glm/glm.hpp>
#include "../ECS.h"

EntityID CreatePlayer(ECS& ecs, glm::vec2 position, glm::vec2 size);
EntityID CreatePlatform(ECS& ecs, glm::vec2 position, glm::vec2 size);
