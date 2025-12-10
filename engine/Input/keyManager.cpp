#include "keyManager.hpp"

KeyManager::KeyManager(GLFWwindow* window){
    windowRef = window;
}

bool KeyManager::isKeyDown(int key){
    return glfwGetKey(windowRef, key);
}