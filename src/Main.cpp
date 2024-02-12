#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Scene.h"

// -------- GLOBALS -------- //

int windowWidth = 1200, windowHeight = 1000;

// -------- CALLBACKS -------- //
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, true);
}

// -------- FUNCTIONS -------- //

GLFWwindow* InitWindow() {
    GLFWwindow* window;
    if (!glfwInit()) {
        std::cout << "Could not initialize GLFW" << std::endl;
        return nullptr;
    }

    window = glfwCreateWindow(windowWidth, windowHeight, "Dodge the bullets!", NULL, NULL);
    if (!window)
    {  
        std::cout << "Could not create window" << std::endl;
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    return window;
}

void InitGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Could not initialize GLAD" << std::endl;
}

// -------- MAIN ------- //

int main()
{
    GLFWwindow* window = InitWindow();
    InitGL();

    Scene scene;

    // Game loop
    float deltaTime = 0.0f; // in seconds
    float last = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Update delta time
        float now = glfwGetTime();
        deltaTime = now - last;
        last = now;

        scene.Update(deltaTime);
        scene.Render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}