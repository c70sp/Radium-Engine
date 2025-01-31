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

#include "./../include/utils.hpp"
#include "./../include/mesh3d.hpp"
#include "./../include/app.hpp"

void Mesh3D::meshCreate(){
    // const std::vector<GLfloat> vertexData{
    //     -0.5f, -0.5f, 0.0f, // BL
    //     0.5f, -0.5f, 0.0f, // BR
    //     -0.5f, 0.5f, 0.0f, // TL
    //     0.5f, 0.5f, 0.0f // TR
    // };
    // const std::vector<GLfloat> vertexData{
    //     0.0f, 0.0f, 0.0f, // BL
    //     150.0f, 0.0f, 0.0f, // BR
    //     0.0f, 150.0f, 0.0f, // TL
    //     150.0f, 150.0f, 0.0f // TR
    // };

    // const std::vector<GLuint> indexBufferData{
    //     2, 0, 1,
    //     3, 2, 1
    // };

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);
    utils::checkGLError("glGenVertexArrays / glBindVertexArray");

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(GLfloat), mVertexData.data(), GL_STATIC_DRAW);
    utils::checkGLError("VBO: glGenBuffers / glBindBuffer / glBufferData");

    glGenBuffers(1, &mIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferData.size() * sizeof(GLuint), mIndexBufferData.data(), GL_STATIC_DRAW);
    utils::checkGLError("IBO: glGenBuffers / glBindBuffer / glBufferData");

    mSize = mIndexBufferData.size();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(GLfloat) * 3, (void*)0);
    utils::checkGLError("glVertexAttribPointer 0");

    // Never fucking switch these two lines, it will break everything. VAA has to be disabled before VAO is unbound.
    // Else: GL_INVALID_OPERATION (0x502 / 1282) error
    // Never fucking mind, if you switch them, the error is gone but nothing is rendered. So, don't switch them hhhhhhh.
    // Alright, you should not unbind the VAA in general, since it is not necessary and makes the GPU ignore it. Just unbind the VAO.

    glBindVertexArray(0);
    utils::checkGLError("After unbinding VAO");
}

void Mesh3D::meshDraw(glm::mat4 projectionMatrix){
    // Bind program
    utils::checkGLError("before useProgram mPipeline");
    glUseProgram(mPipeline);
    utils::checkGLError("after useProgram mPipeline");

    getModelMatrix();

    // Set matrix uniforms
    mApp->mShader.setUniformMat4(mPipeline, "u_ModelMatrix", mModelMatrix);
    utils::checkGLError("setUniformMat4, u_ModelMatrix");

    mApp->mShader.setUniformMat4(mPipeline, "u_ProjectionMatrix", projectionMatrix);
    utils::checkGLError("setUniformMat4, u_ProjectionMatrix");

    mApp->mShader.setUniformVec3(mPipeline, "u_Color", mColor);
    utils::checkGLError("setUniformVec3, u_Color");

    // Bind and draw VAO
    glBindVertexArray(mVertexArrayObject);
    utils::checkGLError("glBindVertexArray");

    glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, 0);
    utils::checkGLError("glDrawElements");

    // Unbind
    glBindVertexArray(0);
    glUseProgram(0);
}

void Mesh3D::destroy(){
    glDeleteBuffers(1, &mVertexBufferObject);
    utils::checkGLError("glDeleteBuffers");

    glDeleteVertexArrays(1, &mVertexArrayObject);
    utils::checkGLError("glDeleteVertexArrays");

    if(mPipeline){
        glDeleteProgramPipelines(1, &mPipeline);
        utils::checkGLError("glDeleteProgramPipelines");
    }
}

void Mesh3D::setPipeline(const std::string& vertexShaderName, const std::string& fragmentShaderName){
    utils::checkGLError("before getPipeline");
    mPipeline = mApp->mShader.getPipeline(vertexShaderName, fragmentShaderName);
    utils::checkGLError("after getPipeline");

    // std::cout << "Pipeline set: " << mPipeline << std::endl;
}

void Mesh3D::setColor(float r, float g, float b){
    if(r >= 0.0f && r <= 1.0f && g >= 0.0f && g <= 1.0f && b >= 0.0f && b <= 1.0f){
        mColor = glm::vec3(r, g, b);
    }else{
        std::cerr << "Invalid mesh color!" << std::endl;
    }
}

void Mesh3D::setColor(int r, int g, int b){
    if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255){
        mColor = glm::vec3((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
    }else{
        std::cerr << "Invalid mesh color!" << std::endl;
    }
}

void Mesh3D::getModelMatrix(){
    if(mType == "circle"){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(mX, (float)mApp->mScreenHeight - mY, mZ));
        setModelMatrix(model);
    }
}