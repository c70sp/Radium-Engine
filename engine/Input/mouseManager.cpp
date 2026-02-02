#include "mouseManager.hpp"

MouseManager::MouseManager(GLFWwindow& window, EventSystem& es)
    : windowRef(window), eventSystemRef(es)
{
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(&window, scroll_callback);
    glfwSetMouseButtonCallback(&window, button_callback);
    glfwSetCursorPosCallback(&window, move_callback);
}

// Mouse movement
glm::vec2 MouseManager::getMousePos(){
    double xpos, ypos;
    glfwGetCursorPos(&windowRef, &xpos, &ypos);

    return glm::vec2(xpos, ypos);
}

// vvvvvvvvvvvvvvvvvvvv Scrolling Callback vvvvvvvvvvvvvvvvvvvv
void MouseManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));

    if(ctx && ctx->mouse) ctx->mouse->onScroll(xoffset, yoffset);
}

void MouseManager::onScroll(double xoffset, double yoffset){
    scrolling = yoffset;
    
    eventSystemRef.emit(MouseScrollEvent{xoffset, yoffset});
}

int MouseManager::getMouseScroll(){
    int delta = scrolling;
    scrolling = 0;
    return delta;
}

// ^^^^^^^^^^^^^^^^^^^^ Scrolling Callback ^^^^^^^^^^^^^^^^^^^^
// vvvvvvvvvvvvvvvvvvvv Mouse Button Callback vvvvvvvvvvvvvvvvvvvv

void MouseManager::button_callback(GLFWwindow* window, int button, int action, int mods){
    auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));

    if(ctx && ctx->mouse) ctx->mouse->onButton(button, action, mods);
}

void MouseManager::onButton(int button, int action, int mods){
    eventSystemRef.emit(MouseButtonEvent{button, action, mods});
}

// ^^^^^^^^^^^^^^^^^^^^ Mouse Button Callback ^^^^^^^^^^^^^^^^^^^^
// vvvvvvvvvvvvvvvvvvvv Mouse Move Callback vvvvvvvvvvvvvvvvvvvv

void MouseManager::move_callback(GLFWwindow* window, double xpos, double ypos){
    auto* ctx = static_cast<WindowContext*>(glfwGetWindowUserPointer(window));

    if(ctx && ctx->mouse) ctx->mouse->onMove(xpos, ypos);
}

void MouseManager::onMove(double xpos, double ypos){
    eventSystemRef.emit(MouseMoveEvent{xpos, ypos});
}

// ^^^^^^^^^^^^^^^^^^^^ Mouse Move Callback ^^^^^^^^^^^^^^^^^^^^
