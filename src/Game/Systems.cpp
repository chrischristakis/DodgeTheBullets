#include "Systems.h"

#include <glm/glm.hpp>
#include "../Components/Component.h"
#include "CollisionUtil.h"

namespace Systems {

	void Move(ECS& ecs, EntityID id, float deltaTime) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		Physics& physics = ecs.GetComponent<Physics>(id);

		transform.position += physics.velocity * deltaTime;

		// Sync the box collider to the entity, if the entity has one.
		if (ecs.HasComponent<BoxCollider>(id)) {
			BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);

			collider.position = transform.position;
		}
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

	void RenderQuad(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		Renderable& renderable = ecs.GetComponent<Renderable>(id);

		renderer.RenderQuad(shader, transform.position, transform.scale, renderable.color);
	}

	void RenderBoxCollider(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader) {
		BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);

		renderer.RenderQuadOutline(shader, collider.position, collider.scale, { 0, 1, 1 }, 2.0f);
	}

	void HandleSolidCollisions(ECS& ecs, EntityID id) {
		BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);

		for (EntityID other : ecs.GetAllActiveIDs<BoxCollider>()) {
			if (other == id)
				continue;

			if (ecs.HasComponent<Renderable>(id)) {
				BoxCollider& colliderOther = ecs.GetComponent<BoxCollider>(other);
				Renderable& renderable = ecs.GetComponent<Renderable>(id);

				if (Collision::CheckSimpleAABB(collider, colliderOther))
					renderable.color = { 1, 0, 0 };
				else
					renderable.color = { 1, 1, 1 };
			}
		}
	}

}