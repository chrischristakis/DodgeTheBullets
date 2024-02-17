#pragma once

#include <memory>
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Core/Camera.h"

class Context {
public:
	static void CreateWindow(Window* window);
	static void CreateAudio(Audio* audio);
	static void CreateCamera(Camera* camera);

	static Window* GetWindow();
	static Audio* GetAudio();
	static Camera* GetCamera();

private:

	inline static std::unique_ptr<Window> m_windowInstance;
	inline static std::unique_ptr<Audio> m_audioInstance;
	inline static std::unique_ptr<Camera> m_cameraInstance;
};