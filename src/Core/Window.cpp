#include "Window.h"
#include <iostream>

Window::Window(int width, int height, std::string title) {
    if (!m_glfwInitialized && !glfwInit()) {
        std::cout << "Could not initialize GLFW" << std::endl;
        return;
    }

    m_glfwInitialized = true;

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_window) {
        std::cout << "Could not create window with title " << title << std::endl;
        return;
    }

    glfwMakeContextCurrent(m_window);
}

Window::~Window() {
	glfwDestroyWindow(m_window);  // Releases window and resources
	m_window = nullptr;
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_window);
}

void Window::Close() {
    glfwSetWindowShouldClose(m_window, true);
}

GLFWwindow* Window::GetNativeWindow() {
	return m_window;
}

void Window::EnableVsync(bool vsync) {
    glfwSwapInterval((vsync) ? 1 : 0);
}

void Window::AttachResizeCallback(GLFWwindowsizefun callback) {
    glfwSetWindowSizeCallback(m_window, callback);
}