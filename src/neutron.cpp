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
#include <memory>

// Own libs
#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"
#include "./../include/neutron.hpp"

Neutron::Neutron(App* app, float x, float y, float velocity, float azimuth){
    mApp = app;
    mX = x;
    mY = y;
    mVelocity = velocity;
    mAzimuth = azimuth;


    mRenderObj = mApp->createCircle(x, y, 5.0f, 16);
    mRenderObj->setPipeline("v_vert.glsl", "f_frag.glsl");
    mRenderObj->meshCreate();
    mRenderObj->setZ(mZ);    
    mRenderObj->setColor(mColor.r, mColor.g, mColor.b);
}

void Neutron::draw(){
    mRenderObj->mX = mX;
    mRenderObj->mY = mY;

    mApp->mDrawQueue.push_back(mRenderObj);
}

void Neutron::update(){
    float dX = mVelocity * cos(mAzimuth);
    float dY = mVelocity * sin(mAzimuth);

    mX += dX;
    mY += dY;
}