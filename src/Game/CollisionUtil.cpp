#include "CollisionUtil.h"

#include <iostream>
#include <algorithm>

namespace Collision {

	bool CheckSimpleAABB(BoxCollider& a, BoxCollider& b) {
		return (
			(a.position.x + a.size.x) > b.position.x &&
			a.position.x < (b.position.x + b.size.x) &&
			(a.position.y + a.size.y) > b.position.y &&
			a.position.y < (b.position.y + b.size.y)
		);
	}
	
	SweptInfo SweptAABB(BoxCollider& moving, BoxCollider& fixed, glm::vec2 velocity) {
		SweptInfo info;

		// If moving box is stationary, no collision.
		if (velocity.x == 0 && velocity.y == 0) {
			info.collided = false;
			return info;
		}

		// Calculate distance on each axis to entry collision point and exit collision point from moving to fixed.
		// Note that this depends on the direction of the velocity on each axis.
		float entryDistanceX, entryDistanceY;
		float exitDistanceX, exitDistanceY;

		if (velocity.x > 0.0f) {
			entryDistanceX = fixed.position.x - (moving.position.x + moving.size.x);
			exitDistanceX = (fixed.position.x + fixed.size.x) - moving.position.x;
		}
		else if (velocity.x < 0.0f) {
			// Note this produces a negative result. This is intentional, since once we divide by a negative velocity later,
			// we will result in a positive time.
			entryDistanceX = (fixed.position.x + fixed.size.x) - moving.position.x;
			exitDistanceX =  fixed.position.x - (moving.position.x + moving.size.x);
		}
		else {
			// If the box is stationary in X and only moving in Y, if the x axis of the moving box
			// does not intersect witht he x axis of the fixed box, then no collision will occur.
			if (moving.position.x + moving.size.x <= fixed.position.x ||
				moving.position.x >= fixed.position.x + fixed.size.x) {
				info.collided = false;
				return info;
			}
		}

		if (velocity.y > 0.0f) {
			entryDistanceY = fixed.position.y - (moving.position.y + moving.size.y);
			exitDistanceY = (fixed.position.y + fixed.size.y) - moving.position.y;
		}
		else if (velocity.y < 0.0f) {
			entryDistanceY = (fixed.position.y + fixed.size.y) - moving.position.y;
			exitDistanceY = fixed.position.y - (moving.position.y + moving.size.y);
		}
		else {
			if (moving.position.y + moving.size.y <= fixed.position.y ||
				moving.position.y >= fixed.position.y + fixed.size.y) {
				info.collided = false;
				return info;
			}
		}

		// Calculate the time to hit the entry and exit points in each axis by dividing by velocity (t = d/v)
		// This results in a number from [0,1] if the collision point lies along the velocity vector (valid collision)
		constexpr float INF = std::numeric_limits<float>::infinity();

		// WHy we set it to -INF and INF becomes clear when considering the max() and min() call bellow
		float entryTimeX = -INF, entryTimeY = -INF;
		float exitTimeX = INF, exitTimeY = INF;

		if (velocity.x != 0.0f) {
			entryTimeX = entryDistanceX / velocity.x;
			exitTimeX = exitDistanceX / velocity.x;
		}
		if (velocity.y != 0.0f) {
			entryTimeY = entryDistanceY / velocity.y;
			exitTimeY = exitDistanceY / velocity.y;
		}

		// Find which axis takes longer to collide with, which tells us the time we collide with box axis,
		// hence our entry time. The opposite is true for the exit time.
		float entryTime = std::max(entryTimeX, entryTimeY);
		float exitTime = std::min(exitTimeX, exitTimeY);

		// Not colliding in 3 cases:
		// 1) Time to exit is less than time to enter (Shouldn't occur)
		// 2) entry time is not between [0,1]
		if (entryTime > exitTime || entryTime < 0.0f || entryTime > 1.0f) {
			info.collided = false;
			return info;
		}

		// At this point, we know we're colliding and how long alonf the velocity vector we do.
		info.collided = true;
		info.collisionTime = entryTime;

		// Calculate the normal of the edge we collide with. If we collide on the y axis before the x axis, we
		// know that were colliding with a top/bottom edge, then we can use velocity to deduce whether it's top or bottom.
		if (entryTimeX > entryTimeY) {
			if (velocity.x < 0.0f)
				// right edge
				info.collisionNormal = { 1, 0 };
			else
				// left edge
				info.collisionNormal = { -1, 0 };
			
		}
		else {
			if (velocity.y < 0.0f)
				// bottom edge
				info.collisionNormal = { 0, 1 };
			else
				// top edge
				info.collisionNormal = { 0, -1 };
		}

		return info;
	}

	BoxCollider CalculateBroadphase(BoxCollider& moving, glm::vec2 velocity) {
		BoxCollider collider;

		if (velocity.x > 0.0f) {
			collider.position.x = moving.position.x;
			collider.size.x = moving.size.x + velocity.x;
		}
		else {
			collider.position.x = moving.position.x - glm::abs(velocity.x);
			collider.size.x = glm::abs(velocity.x) + moving.size.x;
		}

		if (velocity.y > 0.0f) {
			collider.position.y = moving.position.y;
			collider.size.y = moving.size.y + velocity.y;
		}
		else {
			collider.position.y = moving.position.y - glm::abs(velocity.y);
			collider.size.y = glm::abs(velocity.y) + moving.size.y;
		}

		return collider;
	}

}