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
	glm::vec2 acceleration;
	glm::vec2 drag;  // Drag is friction
	float gravity;

	Physics(glm::vec2 velocity = { 0, 0 }, glm::vec2 acceleration = {0, 0}, glm::vec2 drag = {1.0f, 1.0f}, float gravity = 32.0f) :
		velocity(velocity), acceleration(acceleration), drag(drag), gravity(gravity) { }
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
	glm::vec2 size;

	BoxCollider(glm::vec2 position = { 0, 0 }, glm::vec2 size = { 1, 1 }) :
		position(position), size(size) { }
};

struct JumpComponent : public Component {
	bool jumping;
	bool grounded;

	JumpComponent(bool jumping = false, bool grounded = false):
		jumping(jumping), grounded(grounded) { }
};