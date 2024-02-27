#pragma once

#include <glm/glm.hpp>
#include "../Components/Component.h"

namespace Collision {

	bool CheckSimpleAABB(BoxCollider& a, BoxCollider& b);

	struct SweptInfo {
		bool collided;
		float collisionTime;
		glm::vec2 collisionNormal;
	};
	
	SweptInfo SweptAABB(BoxCollider& moving, BoxCollider& fixed, glm::vec2 velocity);

	/*! Returns a broadphase box represented by a box collider.
	*   Note that this box collider is not registered with ECS and is just a simple rectangle.
	*/
	BoxCollider CalculateBroadphase(BoxCollider& moving, glm::vec2 velocity);

}