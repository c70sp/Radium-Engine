#ifndef ATOM_HPP
#define ATOM_HPP

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

class Atom{
    public:
        Atom(App* app, float x, float y, int type);

        float mX, mY = 0;
        int mZ = 0;
        glm::vec3 mColor = glm::vec3(0.7f, 0.8f, 0.4f);
        glm::vec3 mNewColor = glm::vec3(0.7f, 0.8f, 0.4f);
        // Only if the above color changes, the render object gets modifed and mColor gets updated

        std::shared_ptr<Mesh3D> mRenderObj;

        /**
         * Types:
         * 0: U-235 Atom, can fission
         * 1: U-238 or co., can't fission
         * 2: Xenon
         */
        int mType = 0;

        // ===============

        void draw();

    private:
        App* mApp;
};

#endif // ATOM_HPP