#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Window{
    public:
        Window(int width, int heigth, const std::string& title);
        ~Window();

        bool shouldClose() const;
        void pollEvents() const;
        void swapBuffers() const;

        int mWidth = 0;
        int mHeight = 0;

        GLFWwindow* mWindow;
    private:
};