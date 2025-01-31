#ifndef SHADER_HPP
#define SHADER_HPP

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
#include <unordered_set> // for keeping track of "divs"s
#include <unordered_map>

struct ShaderInfo{
    std::string name;
    GLuint ID;
    GLuint type;
};

struct PipelineInfo{
    std::string name;
    GLuint ID; // (programObject)
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
};

class Shader{
    public:
        // Gets all shaders from shader dir, name = file name, type indicated by name start (v_, f_ or g_)
        void compileAllShaders();
        // Shader();

        /**
         * Checks if pipeline with combo exists
         * if it does -> return ID
         * if it doesn't -> create new one (Shader::createPipeline(ID1, ID2)) and return the ID
         */
        GLuint getPipeline(const std::string& vertexShaderName, const std::string& fragmentShaderName);

        // Setter methodes
        void setUniformInt(GLuint pipeline, const char* name, int value);
        void setUniformFloat(GLuint pipeline, const char* name, float value);
        void setUniformMat4(GLuint pipeline, const char* name, glm::mat4& matrix);
        void setUniformVec3(GLuint pipeline, const char* name, glm::vec3& vector);
    private:
        std::unordered_map<int, ShaderInfo> mShaders;
        std::unordered_map<int, PipelineInfo> mPipelines;

        // Searches mShaders for Shader with that name
        GLuint getShader(const std::string& shadername);

        // Compiles shader from type (e.g. GL_VERTEX_SHADER), shader source code and name, creates new mShaders entry
        void compileShader(GLuint type, const std::string& source, std::string& name);

        // Creates new pipeline with two compiled shaders (via shader ID), creates new mPipelines entry
        GLuint createPipeline(const GLuint vertexShaderID, const GLuint fragmentShaderID);
};

#endif // SHADER_HPP