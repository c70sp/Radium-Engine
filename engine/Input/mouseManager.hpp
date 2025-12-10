#pragma once

// C++ STL
#include <iostream>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class MouseManager{
    public:
        MouseManager(GLFWwindow* window);

        glm::vec2 getMousePos();
        int getMouseScroll();

        // For now only that, mouse movement logic goes in game.
        // Maybe later I can add a proper callback which you set in game to get the movement directly

    private:
        int scrolling = 0;

        GLFWwindow* windowRef;

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void onScroll(double xoffset, double yoffset);
};