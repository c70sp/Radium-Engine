#include <glad/glad.h>
#include "window.hpp"

#include <stdexcept>
#include <iostream>


Window::Window(int width, int height, const std::string& title){
    if(!glfwInit()){
        throw std::runtime_error("Failed to init GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GL_DOUBLEBUFFER, 1);
    glfwWindowHint(GL_DEPTH_BITS, 24);

    glfwWindowHint(GLFW_DECORATED, GL_FALSE);

    mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL); // replace first NULL with glfwGetPrimaryMonitor() for fullscr.
    if(mWindow == NULL){
        throw std::runtime_error("Failed to open GLFW window");
    }

    glfwMakeContextCurrent(mWindow);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        throw std::runtime_error("Failed to init glad");
    }

    glfwSwapInterval(1);

    glfwSetWindowAttrib(mWindow, GLFW_RESIZABLE, false);

    mWidth = width;
    mHeight = height;
}

Window::~Window(){
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

bool Window::shouldClose() const{
    return glfwWindowShouldClose(mWindow);
}

void Window::pollEvents() const{
    glfwPollEvents();
}

void Window::swapBuffers() const{
    glfwSwapBuffers(mWindow);
}