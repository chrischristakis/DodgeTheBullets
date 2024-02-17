#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Context.h"
#include "Core/Audio.h"
#include "Scene.h"

// -------- FUNCTIONS -------- //

void InitGL() {
    // must be called after gl context is current, so after window is created.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Could not initialize GLAD" << std::endl;
}

void Init() {
    Context::CreateWindow(new Window(1200, 1000, "Dodge the bullets!!!"));
    Context::CreateAudio(new Audio(32));

    InitGL();
}

// -------- MAIN ------- //

int main()
{
    Init();

    Scene scene;

    // Game loop
    float deltaTime = 0.0f; // in seconds
    float last = 0.0f;
    while (Context::GetWindow()->IsOpen())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Update delta time
        float now = glfwGetTime();
        deltaTime = now - last;
        last = now;

        // Core updates
        Context::GetAudio()->Update();

        scene.Update(deltaTime);
        scene.Render();

        glfwSwapBuffers(Context::GetWindow()->GetNativeWindow());
    }

    glfwTerminate();
    return 0;
}