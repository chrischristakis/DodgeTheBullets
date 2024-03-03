#pragma once

#include <glm/glm.hpp>

class Camera {
public:

	Camera(float zoom, float targetAspect, float windowWidth, float windowHeight);

	glm::mat4 GetViewProjectionMatrix();

	// Should be called when window is resized 
	void OnResize(int width, int height);

	void SetZoom(float zoom);
	float GetZoom();

	float GetHalfWidth();
	float GetHalfHeight();
	glm::vec2 GetPosition();

	void Move(float dx, float dy);

private:

	glm::mat4 m_projection;

	float m_targetAspect;
	float m_zoom;

	glm::vec2 m_position;

};