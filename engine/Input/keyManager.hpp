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

class KeyManager{
    public:
        KeyManager(GLFWwindow& window, EventSystem& es);

        bool isKeyDown(int key);

    private:
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void onKey(int key, int scancode, int action, int mods);

        GLFWwindow& windowRef;
        EventSystem& eventSystemRef;
};