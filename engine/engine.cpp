#include "engine.hpp"

Engine* Engine::instance = nullptr;

Engine::Engine(int width, int height) : window(width, height, "Title :D"),
                   keyManager(window.mWindow), mouseManager(window.mWindow), textureManager(), textManager(textureManager)
{
    instance = this;
}; // 1280, 720

void Engine::run(const GameCallbacks& game){
    if(game.onInit) game.onInit();
    glViewport(0, 0, window.mWidth, window.mHeight);

    while(!window.shouldClose()){
        time.update();
        window.pollEvents();

        renderer.beginFrame();
        if(game.onUpdate) game.onUpdate(time.deltaTime());
        if(game.onRender) game.onRender();
        renderer.endFrame();

        window.swapBuffers();
    }

    if(game.onShutdown) game.onShutdown();
}