#include "CollisionUtil.h"

namespace Collision {

	bool CheckSimpleAABB(BoxCollider& a, BoxCollider& b) {
		return (
			(a.position.x + a.scale.x / 2.0f) > (b.position.x - b.scale.x / 2.0f) &&
			(a.position.x - a.scale.x / 2.0f) < (b.position.x + b.scale.x / 2.0f) &&
			(a.position.y + a.scale.y / 2.0f) > (b.position.y - b.scale.y / 2.0f) &&
			(a.position.y - a.scale.y / 2.0f) < (b.position.y + b.scale.y / 2.0f)
		);
	}

}