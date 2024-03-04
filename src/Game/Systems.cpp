#include "Systems.h"

#include <glm/glm.hpp>
#include <vector>
#include "CollisionUtil.h"
#include "../Components/Component.h"
#include "../Context.h"

namespace Systems {

	void Move(ECS& ecs, EntityID id, float deltaTime) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		Physics& physics = ecs.GetComponent<Physics>(id);

		transform.position += physics.velocity * deltaTime;

		// Sync the box collider to the entity, if the entity has one.
		if (ecs.HasComponent<BoxCollider>(id)) {
			BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);

			collider.position += physics.velocity * deltaTime;
		}
	}

	void ApplyForces(ECS& ecs, EntityID id, float deltaTime) {
		Physics& physics = ecs.GetComponent<Physics>(id);

		physics.velocity.x *= pow(physics.drag.x, deltaTime);
		physics.velocity.y *= pow(physics.drag.y, deltaTime);
		physics.velocity.y += physics.gravity * deltaTime;
	}

	void ProcessMovementInput(ECS& ecs, EntityID id, Window& window, float deltaTime) {
		Physics& physics = ecs.GetComponent<Physics>(id);
		JumpComponent& jumpComponent = ecs.GetComponent<JumpComponent>(id);

		glm::vec2 speed(0, 0);
		const float SPEED = 9.0f;

		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_A) == GLFW_PRESS)
			speed.x = -SPEED;
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_D) == GLFW_PRESS)
			speed.x = SPEED;

		// Jumping
		if (glfwGetKey(window.GetNativeWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (!jumpComponent.jumping && jumpComponent.grounded) {
				jumpComponent.jumping = true;
				jumpComponent.grounded = false;
				physics.velocity.y = -18.0f;
			}
		}

		physics.velocity.x = speed.x;
	}

	void RenderQuad(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		Renderable& renderable = ecs.GetComponent<Renderable>(id);

		renderer.RenderQuad(shader, transform.position, transform.scale, renderable.color);
	}

	void RenderBoxCollider(ECS& ecs, EntityID id, Renderer& renderer, Shader& shader) {
		BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);

		renderer.RenderQuadOutline(shader, collider.position, collider.size, { 0, 1, 1 }, 2.0f);
	}

	void HandleSolidCollisions(ECS& ecs, EntityID id, float deltaTime) {
		Transform& transform = ecs.GetComponent<Transform>(id);
		BoxCollider& collider = ecs.GetComponent<BoxCollider>(id);
		Physics& physics = ecs.GetComponent<Physics>(id);

		// Start by assuming we're not grounded, and set it to true if we do collide with the top edge of something
		if (ecs.HasComponent<JumpComponent>(id)) {
			JumpComponent& jumpComponent = ecs.GetComponent<JumpComponent>(id);
			jumpComponent.grounded = false;
		}

		// Find all colliders the entity collides with, and put their IDs into a list which holds the id and the distance from the entity
		using DistancePair = std::pair<EntityID, float>;
		std::vector<DistancePair> pairs;
		for (EntityID other : ecs.GetAllActiveIDs<BoxCollider>()) {

			// Do not check collision against the entity itself
			if (other == id)
				continue;

			Transform& transformOther = ecs.GetComponent<Transform>(other);
			BoxCollider& colliderOther = ecs.GetComponent<BoxCollider>(other);

			// Do a broadphase check to see if we can avoid the more expansive swept check
			BoxCollider broadphase = Collision::CalculateBroadphase(collider, physics.velocity);
			if (!Collision::CheckSimpleAABB(broadphase, colliderOther))
				continue;

			Collision::SweptInfo info = Collision::SweptAABB(collider, colliderOther, physics.velocity * deltaTime);
			if (info.collided) {
				float distance = glm::distance(transform.position, transformOther.position);
				pairs.push_back({ other, distance });
			}
		}

		// Sort the list based on distance, closest collision goes to the front of the list.
		std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
			return a.second < b.second;
		});

		/* Resolve the collisions in order of distance to entity.
		*  We have to resimulate collisions when doing this, since by resolving the closest one,
		*  we may not encounter the other anymore.
		*  - This feels like an applicable philosophy to problems in life in general no?
		*/ 
		for (auto& [other, dist] : pairs) {

			BoxCollider& colliderOther = ecs.GetComponent<BoxCollider>(other);
			Collision::SweptInfo info = Collision::SweptAABB(collider, colliderOther, physics.velocity * deltaTime);

			if (info.collided) {

				// Resolve by adding an opposing force to the velocity which repels us from entering solids.
				// the opposing force has the polarity of the solid's collision normal.
				float remainingTime = 1.0f - info.collisionTime;
				glm::vec2 opposingForce = info.collisionNormal * glm::abs(physics.velocity) * remainingTime;
				physics.velocity += opposingForce;

				// If entity has a jump component, then notify we are grounded if we collide with the top edge of a box.
				if (info.collisionNormal == glm::vec2(0, -1) && ecs.HasComponent<JumpComponent>(id)) {
					JumpComponent& jumpComponent = ecs.GetComponent<JumpComponent>(id);

					jumpComponent.grounded = true;
					jumpComponent.jumping = false;
				}
			}
		}
	}

	void HandleScreenBounds(ECS& ecs, EntityID player, Camera& camera) {
		Transform& transform = ecs.GetComponent<Transform>(player);
		BoxCollider& collider = ecs.GetComponent<BoxCollider>(player);
		Physics& physics = ecs.GetComponent<Physics>(player);

		float screenRightEdge = camera.GetPosition().x + camera.GetHalfWidth();

		// Clamp the player to the right edge of the screen
		if ((transform.position.x + transform.scale.x) >= screenRightEdge) {
			transform.position.x = screenRightEdge - transform.scale.x;
			collider.position.x = screenRightEdge - transform.scale.x;
			physics.velocity.x = 0.0f;
		}
	}

	void RenderAxis(Renderer& renderer, Shader& shader) {
		renderer.RenderLine(shader, { 0, 0 }, { 1, 0 }, { 1, 0, 0 }, 4.0f);
		renderer.RenderLine(shader, { 0, 0 }, { 0, 1 }, { 0, 1, 0 }, 4.0f);
	}

	void HandlePlayerPassesDeathwall(ECS& ecs, EntityID player, EntityID wall) {
		Transform& playerTransform = ecs.GetComponent<Transform>(player);
		Transform& wallTransform = ecs.GetComponent<Transform>(wall);

		float leeway = playerTransform.scale.x;

		// if player falls behind the deathwall, then end the game.
		if ((playerTransform.position.x + leeway) < (wallTransform.position.x + wallTransform.scale.x)) {
			
		}
	}

	void MoveCameraWithDeathwall(ECS& ecs, Camera& camera, EntityID wall, float deltaTime) {
		Physics& physics = ecs.GetComponent<Physics>(wall);
		camera.Move(physics.velocity.x * deltaTime, physics.velocity.y * deltaTime);
	}

	void PollRespawnPlatform(ECS& ecs, PlatformManager& factory, Camera& camera, EntityID platform, EntityID deathwall) {
		Transform& platformTransform = ecs.GetComponent<Transform>(platform);
		Transform& wallTransform = ecs.GetComponent<Transform>(deathwall);

		// If the platform breaches the deathwall, then we reset it to a random y value at the edge of the screen.
		if ((platformTransform.position.x + platformTransform.scale.x) < (wallTransform.position.x + wallTransform.scale.x)) {
			BoxCollider& platformCollider = ecs.GetComponent<BoxCollider>(platform);

			platformTransform.position.x = camera.GetPosition().x + camera.GetHalfWidth();
			platformCollider.position.x = camera.GetPosition().x + camera.GetHalfWidth();
		}
	}

}