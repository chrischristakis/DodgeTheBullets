#pragma once

#include <glm/glm.hpp>
#include "Component.h"

struct Transform : public Component {
	glm::vec2 position, scale;

	Transform(glm::vec2 position = {0, 0}, glm::vec2 scale = {1, 1}) :
		position(position), scale(scale) { }
};