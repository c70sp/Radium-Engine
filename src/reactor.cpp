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
#include <chrono>
#include <thread>

// Own libs
#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"
#include "./../include/reactor.hpp"
#include "./../include/neutron.hpp"
#include "./../include/atom.hpp"
#include "./../include/controlRod.hpp"
#include "./../include/structs.hpp"

const int targetFPS = 60;
const int frameDelay = 1000 / targetFPS;

Reactor::Reactor(){
    
}

void Reactor::init(App* app){
    mAppRef = app;

    std::vector<int> fuelRodX;
    float radius = 10.0f;
    float diameter = 20.0f;
    float padding = 10.0f;

    float offsetX = 150.0f;
    float offsetY = 0.0f;

    int reactW = mAmountX * diameter + (mAmountX - 1) * padding;
    int reactH = mAmountY * diameter + (mAmountY - 1) * padding;

    float reactX = 640.0f / 2 - reactW / 2;
    float reactY = 480.0f / 2 - reactH / 2;

    for(int row = 0; row < mAmountY; ++row){
        for(int col = 0; col < mAmountX; ++col){
            float x = (reactX + (diameter * col) + ((col) * padding)) + offsetX;
            float y = (reactY + (diameter * row) + ((row + 1) * padding)) + offsetY;

            int type = 0;
            type = utils::getRandomInt(0, 9) <= 1 ? 0 : 1;

            // ===============

            if(col % 3 == 0 && col != 0 && mControlRodsEnabled == true){
                // Create a control rod
                auto it = find(fuelRodX.begin(), fuelRodX.end(), (col * (diameter + padding) + 350) - (radius / 2));
                if(it == fuelRodX.end()) fuelRodX.push_back((col * (diameter + padding) + 350) - (radius / 2));
            }else{
                std::shared_ptr<Atom> atom = std::make_shared<Atom>(mAppRef, x, y, type);
                mAtoms.push_back(atom);
            }
        }
    }

    for(int col : fuelRodX){
        std::shared_ptr<ControlRod> controlRod = std::make_shared<ControlRod>(mAppRef, (float)col, 100.0f, 10.0f, 150.0f);
        mControlRods.push_back(controlRod);
    }

    std::shared_ptr<Neutron> neutron = std::make_shared<Neutron>(mAppRef, 10.0f, 250.0f, 0.6f, 0.0f);
    mNeutrons.insert(neutron);

    std::shared_ptr<Neutron> neutron2 = std::make_shared<Neutron>(mAppRef, -70.0f, 250.0f, 0.6f, 0.0f);
    mNeutrons.insert(neutron2);
}

void Reactor::mainLoop(){
    while(!glfwWindowShouldClose(mAppRef->mWindow)){
        // ==============================================
        // vvvvvvvvvvvvvvv MAIN LOOP CODE vvvvvvvvvvvvvvv


        for(const std::shared_ptr<Atom>& atom : mAtoms){
            atom->draw();
        }

        for(auto it = mNeutrons.begin(); it != mNeutrons.end(); ){
            std::shared_ptr<Neutron> neutron =  *it;

            if(!checkCollision(neutron)){
                neutron->update();
                neutron->draw();
                ++it; // Only increment if no collision was detected for the previous neutron
            }else{
                it = mNeutrons.erase(it); // erase returns the next valid iterator
            }
        }

        for(const std::shared_ptr<ControlRod>& controlRod : mControlRods){
            controlRod->draw();
        }

        mAppRef->draw();

        // ^^^^^^^^^^^^^^^ MAIN LOOP CODE ^^^^^^^^^^^^^^^
        // ====== Chrono stuff to reduce CPU usage ======
        /**
         * Should do something here with chrono and thread::sleep_for,
         * that does decrease CPU usage by a lot, though problem with that is,
         * it causes stuttering.
         * ChatGPT told me to "Combine Sleeping and Busy-Waiting"
         * So what the code was supposed to do is while the time left until 16 ms is hit is still bigger than 1 ms,
         * it should just sleep for 1 ms, once the timeLeft is smaller than 1 ms it goes into a busy loop.
         * Busy loop means an empty while loop until the time left is 0.
         * Still stuttered a bit too much for my liking though.
         * https://chatgpt.com/c/6769c491-9118-8011-a301-57beb1442b37
         */
    }
}

bool Reactor::checkCollision(std::shared_ptr<Neutron> neutron){
    Circle neutronCircle = {(int)neutron->mX, (int)neutron->mY, 5};

    for(auto it = mAtoms.begin(); it != mAtoms.end(); ){
        std::shared_ptr<Atom> atom = *it;

        Circle atomCircle = {(int)atom->mX, (int)atom->mY, 10};
        bool collision = mAppRef->ccCollision(neutronCircle, atomCircle);

        if(collision){
            // std::cout << "Collision between N: " << neutron << " and A: " << atom << std::endl;

            if(atom->mType == 0){
                //? FISSION
                atom->mType = 1; // Change to unreactive atom (soon also maybe Xenon)
                atom->mNewColor = glm::vec3(0.4f, 0.4f, 0.4f);
                fission(atom->mX, atom->mY);

                return true;
            }else{
                //? NO FISSION
                ++it; // (return false, but we do not want to stop looking, so just increment and keep looking)
            }
        }else{
            ++it;
        }
    }

    for(auto it = mControlRods.begin(); it != mControlRods.end(); ){
        std::shared_ptr<ControlRod> controlRod = *it;

        // dist < +- radius von neutron = hit
        if(neutronCircle.x >= controlRod->mX - controlRod->mW / 2 &&
           neutronCircle.x <= controlRod->mX + controlRod->mW / 2 &&
           neutronCircle.y >= controlRod->mY &&
           neutronCircle.y <= controlRod->mY + controlRod->mH){
            std::cout << "Collision between N: " << neutron << " and CR: " << controlRod << std::endl;
            // it = mControlRods.erase(it);
            return true;
        }else{
            ++it;
        }
    }

    return false;
}

// Create 3 new Neutrons and send them into different directions.
void Reactor::fission(float x, float y){
    for(int i = 0; i < 3; i++){
        float vel = 0.6f;
        float azimuth = utils::getRandomFloat(0, 6.28318);

        std::shared_ptr<Neutron> neutron = std::make_shared<Neutron>(mAppRef, x, y, vel, azimuth);
        mNeutrons.insert(neutron);
    }
}