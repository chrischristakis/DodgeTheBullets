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
    Context::CreateCamera(new Camera(12.0f, 16.0f/9.0f, 1920, 1080));

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
    bool limitFps = false;
    constexpr float ticksPerSecond = 60.0f;
    constexpr float frameLimit = 10.0f;
    float fpsLimitDelay = 1 / frameLimit;
    float secondsPerTick = 1 / ticksPerSecond;
    float deltaTime = 0.0f;
    float last = 0.0f;
    float accumulator = 0.0f; // in seconds
    while (Context::GetWindow()->IsOpen())
    {
        // Escape to exit window, outside of loop so we can escape instantly
        glfwPollEvents();
        if (glfwGetKey(Context::GetWindow()->GetNativeWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            Context::GetWindow()->Close();

        float now = glfwGetTime();

        // Allows us to simulate low fps
        if (limitFps && (now - last) < fpsLimitDelay)
            continue;

        glClear(GL_COLOR_BUFFER_BIT);

        // Update delta time
        deltaTime = now - last;
        accumulator += deltaTime;
        last = now;

        // Core updates (Fixed timstep)
        while (accumulator >= secondsPerTick) {
            Context::GetAudio()->Update();

            scene->Update(secondsPerTick);
            accumulator -= secondsPerTick;
        }
        scene->Render();

        glfwSwapBuffers(Context::GetWindow()->GetNativeWindow());
    }

    delete scene;
    glfwTerminate();
    return 0;
}