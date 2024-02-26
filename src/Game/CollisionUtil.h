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

}