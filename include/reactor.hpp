#ifndef REACTOR_HPP
#define REACTOR_HPP

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
#include <unordered_set> // for keeping track of "divs"
#include <memory>

// Own libs
#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"
#include "./../include/neutron.hpp"
#include "./../include/atom.hpp"
#include "./../include/controlRod.hpp"

class Reactor{
    public:
        Reactor();

        void init(App* app);
        void mainLoop();

        bool checkCollision(std::shared_ptr<Neutron> neutron);

        void fission(float x, float y);

        // std::shared_ptr<Mesh3D> mAtom;
        std::shared_ptr<Mesh3D> mNeutron;

        // Switch to unordered_set's to delete with O(1) for max speed and also to make deleting easier
        std::vector<std::shared_ptr<Atom>> mAtoms;
        std::unordered_set<std::shared_ptr<Neutron>> mNeutrons;

        std::vector<std::shared_ptr<ControlRod>> mControlRods;

    private:
        App* mAppRef;
        bool mControlRodsEnabled = false;

        int mAmountX = 10;
        int mAmountY = 10;
};

#endif // REACTOR_HPP