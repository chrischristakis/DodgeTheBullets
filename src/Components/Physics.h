#pragma once

#include "Component.h"

struct Physics: public Component {
	float velX, velY;

	Physics(float velX = 0.0f, float velY = 0.0f) :
		velX(velX), velY(velY) { }
};