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
#include <filesystem>

// Own libs
#include "./../include/shader.hpp"
#include "./../include/utils.hpp"

// Gets all shaders from shader dir, name = file name, type indicated by name start (v_, f_ or g_)
void Shader::compileAllShaders(){
    std::string path = "./../shaders/";
    std::vector<std::string> files;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_regular_file(entry)) { // Check if it's a regular file
                std::string file = entry.path().filename().string();
                files.push_back(file);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    for(std::string file : files){
        std::string fileSource = utils::loadFileAsString("./../shaders/" + file);

        if(file.rfind("v_", 0) == 0){
            compileShader(GL_VERTEX_SHADER, fileSource, file);
        }else if(file.rfind("f_", 0) == 0){
            compileShader(GL_FRAGMENT_SHADER, fileSource, file);
        }
    }
}

// Compiles shader from type (e.g. GL_VERTEX_SHADER), shader source code and name, creates new mShaders entry
void Shader::compileShader(GLuint type, const std::string& source, std::string& name){
    std::cout << "Compiling type " << type << " shader" << std::endl;
    GLuint shaderObject;

    if(type == GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
        utils::checkGLError("glCreateShader, GL_VERTEX_SHADER");
    }else if(type == GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        utils::checkGLError("glCreateShader, GL_FRAGMENT_SHADER");
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    utils::checkGLError("set glShaderSource");
    
    glCompileShader(shaderObject);
    utils::checkGLError("glCompileSource");

    GLint compileStatus;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(shaderObject, logLength, &logLength, log.data());
        std::cerr << "Shader compilation failed: " << log.data() << std::endl;
    }

    ShaderInfo shader;
    shader.name = name;
    shader.ID = shaderObject;
    shader.type = type;
    int id = mShaders.size();

    mShaders.emplace(id, shader);
}

// Creates new pipeline with two compiled shaders (via shader ID) creates new mPipeliens entry
GLuint Shader::createPipeline(const GLuint vertexShaderID, const GLuint fragmentShaderID){
    std::cout << "Pipeline not found, creating new one..." << std::endl;
    GLuint programObject = glCreateProgram();
    utils::checkGLError("glCreateProgram");

    glAttachShader(programObject, vertexShaderID);
    glAttachShader(programObject, fragmentShaderID);
    utils::checkGLError("glAttachShader");

    glLinkProgram(programObject);
    utils::checkGLError("glLinkProgram");

    GLint linkStatus;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetProgramInfoLog(programObject, logLength, &logLength, log.data());
        std::cerr << "Program linking failed: " << log.data() << std::endl;
    }

    glValidateProgram(programObject);
    utils::checkGLError("glValidateProgram");

    // Make pipeline entry into map
    PipelineInfo pipeline;
    pipeline.name = programObject;
    pipeline.ID = programObject;
    pipeline.vertexShaderID = vertexShaderID;
    pipeline.fragmentShaderID = fragmentShaderID;
    int id = mPipelines.size();
    mPipelines.emplace(id, pipeline);
    std::cout << "Pipeline " << pipeline.ID << " created!" << std::endl;

    return programObject;
}

/**
 * Checks if pipeline with combo exists
 * if it does -> return ID
 * if it doesn't -> create new one (Shader::createPipeline(ID1, ID2)) and return the ID
 */
GLuint Shader::getPipeline(const std::string& vertexShaderName, const std::string& fragmentShaderName){
    utils::checkGLError("getPipeline, before getShader");
    GLuint vertexShaderID = getShader(vertexShaderName);
    utils::checkGLError("getShader, vertexShaderName");

    utils::checkGLError("getPipeline, before getShader");
    GLuint fragmentShaderID = getShader(fragmentShaderName);
    utils::checkGLError("getShader, fragmentShaderName");

    for(auto& [id, pipeline] : mPipelines){
        if(pipeline.vertexShaderID == vertexShaderID && pipeline.fragmentShaderID == fragmentShaderID){
            // std::cout << "Found requested pipeline: " << pipeline.ID << std::endl;
            return pipeline.ID; // ID = programObject
        }
    }
    
    return createPipeline(vertexShaderID, fragmentShaderID);
};

// Searches mShaders for Shader with that name
GLuint Shader::getShader(const std::string& shaderName){
    utils::checkGLError("Before retrieving shader pointer (information)");
    ShaderInfo* foundShader;
    utils::checkGLError("After retrieving shader pointer (information)");

    for(auto& [id, shader] : mShaders){
        if(shader.name == shaderName){
            foundShader = &shader;
            break;
        }
    }

    if(foundShader){
        // std::cout << "Found requested shader: " << foundShader->ID << " | " << foundShader->name << " | " << foundShader->type << std::endl;
        utils::checkGLError("getShader, after finding shader");
        return foundShader->ID;
    }else{
        std::cerr << "Shader with name \"" << shaderName << "\" not found!" << std::endl;
        utils::checkGLError("getShader, after shader not found");
        return 0;
    }
}

// vvvvvvvvvv Uniform setters vvvvvvvvvv

// Statt GLuint einfach map ID? Müsste dafür aber den Struct inkl. ID bzw. nen std::pair ans Mesh schicken. Für jetzt reicht es.
void Shader::setUniformInt(GLuint pipeline, const char* name, int value){
    GLint location = glGetUniformLocation(pipeline, name);
    utils::checkGLError("glGetUniformLocation, setUniformInt");
    glUniform1i(location, value);
    utils::checkGLError("glUniform1i, setUniformInt");
}

void Shader::setUniformFloat(GLuint pipeline, const char* name, float value){
    GLint location = glGetUniformLocation(pipeline, name);
    utils::checkGLError("glGetUniformLocation, setUniformFloat");
    glUniform1f(location, value);
    utils::checkGLError("glUniform1f, setUniformFloat");
}

void Shader::setUniformMat4(GLuint pipeline, const char* name, glm::mat4& matrix){
    GLint location = glGetUniformLocation(pipeline, name);
    utils::checkGLError("glGetUniformLocation, setUniformMat4");
    glUniformMatrix4fv(location, 1, false, &matrix[0][0]);
    utils::checkGLError("glUniformMatrix4fv, setUniformMat4");
}

void Shader::setUniformVec3(GLuint pipeline, const char* name, glm::vec3& vec){
    GLint location = glGetUniformLocation(pipeline, name);
    utils::checkGLError("glGetUniformLocation, setUniformVec3");
    glUniform3fv(location, 1, &vec[0]);
    utils::checkGLError("glUniform3fv, setUniformVec3");
}