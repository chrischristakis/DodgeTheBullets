#pragma once

#include <glm/glm.hpp>

class Camera {
public:

	Camera(float zoom, float targetAspect, float windowWidth, float windowHeight);

	glm::mat4 GetProjectionMatrix();

	// Should be called when window is resized 
	void OnResize(int width, int height);

	void SetZoom(float zoom);

private:

	glm::mat4 m_projection;

	float m_targetAspect;
	float m_zoom;

};