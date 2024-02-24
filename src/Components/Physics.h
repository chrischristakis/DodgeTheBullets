#pragma once

#include <glm/glm.hpp>
#include "Component.h"

struct Physics: public Component {
	glm::vec2 velocity;

	Physics(glm::vec2 velocity = { 0, 0 }) :
		velocity(velocity) { }
};