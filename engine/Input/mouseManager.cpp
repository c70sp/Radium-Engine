#include "mouseManager.hpp"

MouseManager::MouseManager(GLFWwindow* window){
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    windowRef = window;

    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, scroll_callback);
}

// Mouse movement
glm::vec2 MouseManager::getMousePos(){
    double xpos, ypos;
    glfwGetCursorPos(windowRef, &xpos, &ypos);

    return glm::vec2(xpos, ypos);
}

// Scrolling
void MouseManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    auto* manager = static_cast<MouseManager*>(glfwGetWindowUserPointer(window));

    if(manager) manager->onScroll(xoffset, yoffset);
}

void MouseManager::onScroll(double xoffset, double yoffset){
    scrolling = yoffset;
}

int MouseManager::getMouseScroll(){
    int delta = scrolling;
    scrolling = 0;
    return delta;
}