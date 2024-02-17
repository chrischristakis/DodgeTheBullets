#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window {
public:

	Window(int width, int height, std::string title = "Game");
	~Window();
	GLFWwindow* GetNativeWindow();

	bool IsOpen();

private:
	inline static bool m_glfwInitialized = false;
	GLFWwindow* m_window = nullptr;
};