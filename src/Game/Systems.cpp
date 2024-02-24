#include "Systems.h"

#include <glm/glm.hpp>
#include "../Components/Common.h"

namespace Systems {

	void Move(ECS& ecs, EntityID id, float deltaTime) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		Physics& physics = ecs.GetComponent<Physics>(id);

		transform.position += physics.velocity * deltaTime;
	}

	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window) {
		Physics& physics = ecs.GetComponent<Physics>(id);
		glm::vec2 resultant(0, 0);
		constexpr float SPEED = 3.0f;

		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_A) == GLFW_PRESS)
			resultant.x += -SPEED;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_D) == GLFW_PRESS)
			resultant.x +=  SPEED;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_W) == GLFW_PRESS)
			resultant.y += -SPEED;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_S) == GLFW_PRESS)
			resultant.y +=  SPEED;

		physics.velocity = resultant;
	}

}