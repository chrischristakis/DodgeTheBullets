#pragma once

struct Transform {
	float x, y;
	float scaleX, scaleY;

	Transform(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f):
		x(x), y(y), scaleX(scaleX), scaleY(scaleY) { }
	Transform() : Transform(0, 0, 1, 1) { }
};