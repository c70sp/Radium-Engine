#pragma once

// C++ STL
#include <iostream>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "./../windowContext.hpp"
#include "./../eventSystem/eventSystem.hpp"
class EventSystem;

class MouseManager{
    public:
        MouseManager(GLFWwindow& window, EventSystem& es);

        glm::vec2 getMousePos();
        int getMouseScroll();

        // For now only that, mouse movement logic goes in game.
        // Maybe later I can add a proper callback which you set in game to get the movement directly

    private:
        int scrolling = 0;

        GLFWwindow& windowRef;
        EventSystem& eventSystemRef;

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        void onScroll(double xoffset, double yoffset);

        static void button_callback(GLFWwindow* window, int button, int action, int mods);
        void onButton(int button, int action, int mods);

        static void move_callback(GLFWwindow* window, double xpos, double ypos);
        void onMove(double xpos, double ypos);
};