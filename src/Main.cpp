#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Context.h"
#include "Core/Audio.h"
#include "Scene.h"

// -------- FUNCTIONS -------- //

Scene* scene;

void InitGL() {
    // must be called after gl context is current, so after window is created.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Could not initialize GLAD" << std::endl;
}

void Init() {
    Context::CreateWindow(new Window(1920, 1080, "Dodge the bullets!!!"));
    InitGL();
    Context::CreateAudio(new Audio(32));
    Context::CreateCamera(new Camera(5.0f, 16.0f/9.0f, 1920, 1080));

    scene = new Scene();  // not sure how I feel about this being a global, but needs to be to use in resize callback.

    Context::GetWindow()->AttachResizeCallback([](GLFWwindow* window, int width, int height) {
        Context::GetCamera()->OnResize(width, height);
        
        // Render scene when resizing since glfw freezes by default.
        scene->Render();
        glfwSwapBuffers(window);
    });

    Context::GetWindow()->EnableVsync(false);
}

// -------- MAIN ------- //

int main()
{
    Init();

    // Game loop
    float deltaTime = 0.0f; // in seconds
    float last = 0.0f;
    while (Context::GetWindow()->IsOpen())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Escape to exit window
        if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            Context::GetWindow()->Close();

        // Update delta time
        float now = glfwGetTime();
        deltaTime = now - last;
        last = now;

        // Core updates
        Context::GetAudio()->Update();

        scene->Update(deltaTime);
        scene->Render();

        glfwSwapBuffers(Context::GetWindow()->GetNativeWindow());
    }

    delete scene;
    glfwTerminate();
    return 0;
}