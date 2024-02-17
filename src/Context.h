#pragma once

#include <memory>
#include "Core/Window.h"
#include "Core/Audio.h"

class Context {
public:
	static void CreateWindow(Window* window);
	static void CreateAudio(Audio* audio);

	static Window* GetWindow();
	static Audio* GetAudio();

private:

	inline static std::unique_ptr<Window> m_windowInstance;
	inline static std::unique_ptr<Audio> m_audioInstance;
};