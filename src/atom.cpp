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
#include "./../include/atom.hpp"

Atom::Atom(App* app, float x, float y, int type){
    mApp = app;
    mX = x;
    mY = y;
    mType = type;

    mNewColor = type == 0 ? mColor : glm::vec3(0.4f);

    mRenderObj = mApp->createCircle(x, y, 10.0f, 16);
    // mRenderObj->setZ(mZ);
    mRenderObj->setPipeline("v_vert.glsl", "f_frag.glsl");
    mRenderObj->meshCreate();
    mRenderObj->setColor(mColor.r, mColor.g, mColor.b);
}

void Atom::draw(){
    mRenderObj->mX = mX;
    mRenderObj->mY = mY;

    if(mNewColor != mColor){
        mColor = mNewColor;
        mRenderObj->setColor(mColor.r, mColor.g, mColor.b);
    }

    mApp->mDrawQueue.push_back(mRenderObj);
}