#pragma once

#include <glm/glm.hpp>

// Base class allows component pool abstraction
struct Component {
	virtual ~Component() { }
};

// ----- COMPONENTS ----- //
// Components MUST implement a default constructor, or compiler error

struct Physics : public Component {
	glm::vec2 velocity;

	Physics(glm::vec2 velocity = { 0, 0 }) :
		velocity(velocity) { }
};

struct Transform : public Component {
	glm::vec2 position, scale;

	Transform(glm::vec2 position = { 0, 0 }, glm::vec2 scale = { 1, 1 }) :
		position(position), scale(scale) { }
};

struct Renderable : public Component {
	glm::vec3 color;

	Renderable(glm::vec3 color = {1, 1, 1}) : color(color) { }
};

struct BoxCollider : public Component {
	glm::vec2 position;
	glm::vec2 scale;

	BoxCollider(glm::vec2 position = { 0, 0 }, glm::vec2 scale = { 1, 1 }) :
		position(position), scale(scale) { }
};