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
#include "./../include/controlRod.hpp"

ControlRod::ControlRod(App* app, float x, float y, float w, float h){
    mApp = app;
    mX = x;
    mY = y;
    mW = w;
    mH = h;

    mRenderObj = mApp->createRect(x, y, w, h);
    mRenderObj->setPipeline("v_vert.glsl", "f_frag.glsl");
    mRenderObj->meshCreate();
    mRenderObj->setZ(mZ);
    mRenderObj->setColor(mColor.r, mColor.g, mColor.b);
}

void ControlRod::draw(){
    mRenderObj->mX = mX;
    mRenderObj->mY = mY;

    mApp->mDrawQueue.push_back(mRenderObj);
}