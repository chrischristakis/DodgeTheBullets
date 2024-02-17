#include "Context.h"

void Context::CreateWindow(Window* window) {
	// unique ptr takes ownership of the raw pointer and manages the memory appropriately.
	m_windowInstance = std::unique_ptr<Window>(window);
}

Window* Context::GetWindow() {
	return m_windowInstance.get();
}

void Context::CreateAudio(Audio* audio) {
	m_audioInstance = std::unique_ptr<Audio>(audio);
}

Audio* Context::GetAudio() {
	return m_audioInstance.get();
}

void Context::CreateCamera(Camera* camera) {
	m_cameraInstance = std::unique_ptr<Camera>(camera);
}

Camera* Context::GetCamera() {
	return m_cameraInstance.get();
}