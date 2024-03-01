#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <algorithm>

Camera::Camera(float zoom, float targetAspect, float windowWidth, float windowHeight) :
	m_targetAspect(targetAspect), m_zoom(zoom), m_position(0, 0)
{
	m_projection = glm::ortho(
		-zoom * targetAspect,
		zoom * targetAspect,
		zoom,
		-zoom,
		0.1f,
		100.0f
	);

	OnResize(windowWidth, windowHeight);
}

glm::mat4 Camera::GetViewProjectionMatrix() {
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));

	return m_projection * view;
}

void Camera::Move(float dx, float dy) {
	m_position += glm::vec2(dx, dy);
}

/*! Changes projection matrix to zoom camera in or out, showing more of the game world
*	@param zoom: the higher the value, the more of the game world you see.
*/
void Camera::SetZoom(float zoom) {
	m_zoom = std::max(zoom, 0.01f);

	m_projection = glm::ortho(
		-m_zoom * m_targetAspect,
		m_zoom * m_targetAspect,
		m_zoom,
		-m_zoom,
		0.1f,
		100.0f
	);
}

float Camera::GetZoom() {
	return m_zoom;
}

/*! Resizes the viewport to fit the window dimensions and adds letterboxing
*	@param width: Width of window after resize
*   @param height: Height of window after resize
*/
void Camera::OnResize(int width, int height) {
	float aspect = (float)width / (float)height;

	// Keeps the target aspect ratio intact, centering the viewport in the window.

	if (aspect >= m_targetAspect) {						// Center horizontally
		float scaledWidth = height * m_targetAspect;
		float offsetX = (width - scaledWidth) / 2.0f;
		glViewport(offsetX, 0, scaledWidth, height);
	}
	else{												// Center vertically
		float scaledHeight = width / m_targetAspect;
		float offsetY = (height - scaledHeight) / 2.0f;
		glViewport(0, offsetY, width, scaledHeight);
	}
}

float Camera::GetHalfWidth() {
	return m_zoom * m_targetAspect;
}

float Camera::GetHalfHeight() {
	return m_zoom;
}