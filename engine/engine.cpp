#include "engine.hpp"

Engine* Engine::instance = nullptr;

Engine::Engine(int width, int height) :
                window(width, height, "Title :D"), // 1280, 720
                keyManager(*window.mWindow, eventSystem),
                mouseManager(*window.mWindow, eventSystem),
                textureManager(),
                textManager(textureManager)
{
    instance = this;
    mWindowSize = glm::vec2(width, height);

    WindowContext* ctx = new WindowContext{};
    ctx->mouse = &mouseManager;
    ctx->key = &keyManager;
    glfwSetWindowUserPointer(window.mWindow, ctx);
};

void Engine::run(const GameCallbacks& game){
    if(game.onInit) game.onInit();
    glViewport(0, 0, window.mWidth, window.mHeight);

    while(!window.shouldClose()){
        time.update();
        window.pollEvents();

        eventSystem.process();

        renderer.beginFrame();
        if(game.onUpdate) game.onUpdate(time.deltaTime());
        if(game.onRender) game.onRender();
        renderer.endFrame();

        window.swapBuffers();
    }

    if(game.onShutdown) game.onShutdown();
}