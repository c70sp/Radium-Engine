#pragma once

// C++ STL


// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class KeyManager{
    public:
        KeyManager(GLFWwindow* window);

        bool isKeyDown(int key);

    private:
        GLFWwindow* windowRef;
};