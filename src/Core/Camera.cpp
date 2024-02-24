#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <algorithm>

Camera::Camera(float zoom, float targetAspect): m_targetAspect(targetAspect), m_zoom(zoom) {
	m_projection = glm::ortho(
		-zoom * targetAspect,
		zoom * targetAspect,
		zoom,
		-zoom,
		0.1f,
		100.0f
	);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return m_projection;
}

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