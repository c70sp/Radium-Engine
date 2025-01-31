#ifndef NEUTRON_HPP
#define NEUTRON_HPP

#define GLM_ENABLE_EXPERIMENTAL

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <dwmapi.h>

// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>

// Own libs
#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"

class App;

class Neutron{
    public:
        Neutron(App* app, float x, float y, float velocity, float azimuth);

        float mX, mY = 0;
        int mZ = 1;
        float mVelocity = 0.6;
        float mAzimuth = 0; // in rad

        glm::vec3 mColor = glm::vec3(0.85f, 0.85f, 0.85f);

        std::shared_ptr<Mesh3D> mRenderObj;

        // ===============

        void draw();
        void update();

    private:
        App* mApp;
};

#endif // NEUTRON_HPP