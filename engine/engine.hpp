#define ENGINE Engine::instance
#pragma once

#include <functional>
#include "./core/common.hpp"
#include "./renderer/renderer.hpp"
#include "./core/window.hpp"
#include "./core/time.hpp"
#include "./cameraManager/cameraManager.hpp"
#include "./meshManager/meshManager.hpp"
#include "./shaderManager/shaderManager.hpp"
#include "./input/keyManager.hpp"
#include "./input/mouseManager.hpp"
#include "./collisionDetection/rayCast.hpp"
#include "./collisionDetection/collisionManager.hpp"
#include "./uiManager/uiManager.hpp"
#include "./textureManager/textureManager.hpp"
#include "./textManager/textManager.hpp"

struct GameCallbacks{
    std::function<void()> onInit;
    std::function<void(float)> onUpdate;
    std::function<void()> onRender;
    std::function<void()> onShutdown;
};

class Engine{
    public:
        static Engine* instance;
        Engine(int width, int height);
        void run(const GameCallbacks& game);

        GLFWwindow* getWindow() {return window.mWindow;}

        Renderer& getRenderer() {return renderer;}
        MeshManager& getMeshManager() {return meshManager;}
        ShaderManager& getShaderManager() {return shaderManager;}
        KeyManager& getKeyManager() {return keyManager;}
        MouseManager& getMouseManager() {return mouseManager;}
        RayCast& getRayCast() {return rayCast;}
        Time& getTime() {return time;}
        UIManager& getUIManager() {return uiManager;}
        CameraManager& getCameraManager() {return cameraManager;}
        TextureManager& getTextureManager() {return textureManager;}
        TextManager& getTextManager() {return textManager;}
        CollisionManager& getCollisionManager() {return collisionManager;}
        
        Window window;
    private:
        TextureManager textureManager;
        TextManager textManager;

        Renderer renderer;
        Time time;
        MeshManager meshManager;
        ShaderManager shaderManager;
        CameraManager cameraManager;
        KeyManager keyManager;
        MouseManager mouseManager;
        RayCast rayCast;
        UIManager uiManager;
        CollisionManager collisionManager;
};