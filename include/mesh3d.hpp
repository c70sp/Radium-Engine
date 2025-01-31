#ifndef MESH3D_HPP
#define MESH3D_HPP

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
#include <memory>

// Own libs
#include "./app.hpp"
class App; // forward decleration

class Mesh3D{
    public:
        Mesh3D(App* app) : mApp(app) {};

        //* vvvvvvvvvv Attribs vvvvvvvvvv

        GLuint mVertexArrayObject = 0;
        GLuint mVertexBufferObject = 0;
        GLuint mIndexBufferObject = 0;
        GLuint mPipeline = 0;

        unsigned int mSize = 0;

        glm::mat4 mModelMatrix = glm::mat4(1.0f);

        glm::vec3 mColor = glm::vec3(1.0f, 1.0f, 1.0f); // Normalised (from 0 to 1)

        // mX and mY come from constructor, mZ comes from setter methode
        float mX, mY, mZ = 0;
        float offset = 0;

        //* ^^^^^^^^^^ Attribs ^^^^^^^^^^
        //* vvvvvvvvvv Methodes vvvvvvvvvv

        // Setter methodes
        void setApp(App* app) {mApp = app;}
        void setPipeline(const std::string& vertexShaderName, const std::string& fragmentShaderName);
        void setModelMatrix(glm::mat4 modelMatrix) {mModelMatrix = modelMatrix;}
        void setVertexData(std::vector<GLfloat> vertexData) {mVertexData = vertexData;}
        void setIndexBufferData(std::vector<GLuint> indexBufferData) {mIndexBufferData = indexBufferData;}
        void setType(std::string type) {mType = type;}
        void setZ(int zLayer) {mZ = (float)zLayer;}

        void setColor(float r, float g, float b);
        void setColor(int r, int g, int b);

        void meshCreate();
        void meshDraw(glm::mat4 projectionMatrix);
        void draw();

        void translate(float x, float y, float z);
        void rotate(float angle, glm::vec3 axis);
        void scale(float x, float y, float z);

        void destroy();

        //* ^^^^^^^^^^ Methodes ^^^^^^^^^^
    private:
        App* mApp;
        std::vector<GLfloat> mVertexData{};
        std::vector<GLuint> mIndexBufferData{};

        std::string mType;
        void getModelMatrix();
};

#endif // MESH3D_HPP