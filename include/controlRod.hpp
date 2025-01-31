#ifndef CONTROLROD_HPP
#define CONTROLROD_HPP

#define GLM_ENABLE_EXPERIMENTAL

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class App; // forward decleration

class ControlRod{
    public:
        ControlRod(App* app, float x, float y, float w, float h);

        float mX, mY = 0;
        int mZ = 2;

        float mW, mH = 0;

        glm::vec3 mColor = glm::vec3(0.3f, 0.3f, 0.3f);

        std::shared_ptr<Mesh3D> mRenderObj;

        // ===============

        void draw();
        void update();

    private:
        App* mApp;
};

#endif // CONTROLROD_HPP