#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Window {
public:

	Window(int width, int height, std::string title = "Game");
	~Window();
	GLFWwindow* GetNativeWindow();

	bool IsOpen();
	void Close();

	void AttachResizeCallback(GLFWwindowsizefun);

	void EnableVsync(bool vsync);

private:
	inline static bool m_glfwInitialized = false;
	GLFWwindow* m_window = nullptr;
};