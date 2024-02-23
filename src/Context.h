#pragma once

#include <memory>
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Core/Camera.h"
#include "ECS.h"

class Context {
public:
	static void CreateWindow(Window* window);
	static void CreateAudio(Audio* audio);
	static void CreateCamera(Camera* camera);
	static void CreateECS(ECS* ecs);

	static Window* GetWindow();
	static Audio* GetAudio();
	static Camera* GetCamera();
	static ECS* GetECS();

private:

	inline static std::unique_ptr<Window> m_windowInstance;
	inline static std::unique_ptr<Audio> m_audioInstance;
	inline static std::unique_ptr<Camera> m_cameraInstance;
	inline static std::unique_ptr<ECS> m_ecs;
};