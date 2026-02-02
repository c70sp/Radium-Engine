#include "keyManager.hpp"

KeyManager::KeyManager(GLFWwindow& window, EventSystem& es)
    : windowRef(window), eventSystemRef(es)
{
    glfwSetKeyCallback(&window, key_callback);
};

bool KeyManager::isKeyDown(int key){
    return glfwGetKey(&windowRef, key);
}

void KeyManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));

    if(ctx && ctx->key) ctx->key->onKey(key, scancode, action, mods);
}

void KeyManager::onKey(int key, int scancode, int action, int mods){
    eventSystemRef.emit(KeyPressEvent{key, scancode, action, mods});
}