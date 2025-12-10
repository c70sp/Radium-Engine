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
#include <fstream>

#include "shaderManager.hpp"
#include "./../core/common.hpp"

// ShaderManager::ShaderManager(){
//     ShaderManager::compileAllShaders();
// }

// !? vvvvvvvvvv SHADER CODE vvvvvvvvvv
GLuint ShaderManager::compileShader(GLuint type, const std::string& source, const std::string& name){
    GLuint shaderObject;

    if(type == GL_VERTEX_SHADER){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }else if(type == GL_FRAGMENT_SHADER){
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    GLint success;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed (" << name << "): " << infoLog << std::endl;
    }

    return shaderObject;
}

GLuint ShaderManager::getShaderType(std::string shaderName){
    char typeChar = shaderName[0];

    try{
        switch(typeChar){
            case 'v': return GL_VERTEX_SHADER;
            case 'f': return GL_FRAGMENT_SHADER;
            default: throw std::runtime_error("Unknown shader type");
        }
    }catch(const std::exception& err){
        std::cerr << "Error: " << err.what() << std::endl;
        return GL_VERTEX_SHADER;
    }
}

void ShaderManager::compileAllShaders(){
    std::string path = "C:\\dev\\projects\\C++\\OpenGL\\manIdk\\.shaders\\";
    std::vector<std::string> fileNames;

    for(const auto& entry : std::filesystem::directory_iterator(path)){
        if(entry.is_regular_file()){
            fileNames.push_back(entry.path().filename().string());
        }
    }

    std::string tempSourceCode;
    tempSourceCode.reserve(8192); // reserve 8 KB of memory, makes everything a bit faster
    for(std::string& fname : fileNames){
        ShaderInfo SI;
        SI.type = getShaderType(fname);
        SI.name = fname;
        
        tempSourceCode = loadFileAsString(path + fname);
        SI.id = compileShader(SI.type, tempSourceCode, SI.name);

        mShaders.push_back(SI);
    }
}

// !? vvvvvvvvvv PROGRAM CODE vvvvvvvvvv
ProgramInfo* ShaderManager::findExistingProgram(std::vector<ShaderInfo> shaders, const std::string& name){
    std::unordered_set<std::string> targetNames;
    for(const auto& s : shaders){
        targetNames.insert(s.name);
    }

    for(auto& prog : mShaderPrograms){
        std::unordered_set<std::string> progNames;
        for(const auto& s : prog.programShaders){
            progNames.insert(s.name);
        }

        if(progNames.size() == targetNames.size()){
            bool allMatch = true;
            for(const auto& name : targetNames){
                if(!progNames.count(name)){
                    allMatch = false;
                    break;
                }
            }
            if(allMatch){
                // std::cout << "Found prog!!" << std::endl;
                return &prog;
            }
        }
    }

    return nullptr;
}

ProgramInfo ShaderManager::createNewProgram(std::vector<ShaderInfo> shaders, const std::string& name){
    GLuint programObject = glCreateProgram();
    ProgramInfo prog;
    prog.name = name;

    for(const auto& shader : shaders){
        glAttachShader(programObject, shader.id);
        prog.programShaders.push_back(shader);
    }
    glLinkProgram(programObject);
    
    // check linking
    GLint linkSuccess;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linkSuccess);
    if(!linkSuccess){
        char infoLog[512];
        glGetProgramInfoLog(programObject, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
    }
    
    // Validate & check validation
    glValidateProgram(programObject);
    GLint validateStatus;
    glGetProgramiv(programObject, GL_VALIDATE_STATUS, &validateStatus);
    if(!validateStatus){
        char infoLog[512];
        glGetProgramInfoLog(programObject, 512, nullptr, infoLog);
        std::cerr << "Program validation failed: " << infoLog << std::endl;
    }

    for(const auto& shader : shaders){
        glDetachShader(programObject, shader.id);
        // glDeleteShader(shader.id); // only if you don't plan to reuse it
    }

    prog.id = programObject;
    mShaderPrograms.push_back(prog);
    return prog;
}

ProgramInfo ShaderManager::getShaderProgram(std::vector<std::string> shaderNames, const std::string& name){
    std::unordered_set<std::string> shaderNameSet(shaderNames.begin(), shaderNames.end());
    std::vector<ShaderInfo> shadersForProg;

    for(const auto& shader : mShaders){
        if(shaderNameSet.count(shader.name)){
            shadersForProg.push_back(shader);
            // std::cout << "Found shader: " << shader.name << std::endl;
        }
    }

    // Check for missing shaders
    if (shadersForProg.size() != shaderNames.size()) {
        std::cerr << "[ShaderManager] Error: Missing shaders for program \"" << name << "\"\n";
        for (const auto& requested : shaderNames) {
            bool found = std::any_of(shadersForProg.begin(), shadersForProg.end(), [&](const ShaderInfo& s){ return s.name == requested; });
            if (!found)
                std::cerr << "  - Missing shader: \"" << requested << "\"\n";
        }
        return {}; // return an empty ProgramInfo (id = 0)
    }

    ProgramInfo* existing = findExistingProgram(shadersForProg, name);
    if(existing) return *existing;
    
    return createNewProgram(shadersForProg, name);
}

// vvvvvvvvvvvvvvv UNIFORM STUFF vvvvvvvvvvvvvvv
// Caching the locations for faster access, no need to get them over and over again
GLint ShaderManager::getCachedUniformLocation(GLuint program, const std::string& name){
    auto& programCache = uniformCache[program];
    auto it = programCache.find(name);
    if(it != programCache.end()) return it->second;

    GLint location = glGetUniformLocation(program, name.c_str());
    programCache[name] = location;

    if(location == -1){
        std::cerr << "Uniform: \"" << name << "\" not found in program: \"" << program << "\"" << std::endl;
    }

    return location;
}

void ShaderManager::setUniformInt(GLuint program, const char* name, const int value){
    glUseProgram(program);
    glUniform1i(getCachedUniformLocation(program, name), value);
}

void ShaderManager::setUniformFloat(GLuint program, const char* name, const float value){
    glUseProgram(program);
    glUniform1f(getCachedUniformLocation(program, name), value);
}

void ShaderManager::setUniformVec3(GLuint program, const char* name, const glm::vec3& value){
    glUseProgram(program);
    glUniform3fv(getCachedUniformLocation(program, name), 1, &value[0]);
}

void ShaderManager::setUniformVec4(GLuint program, const char* name, const glm::vec4& value){
    glUseProgram(program);
    glUniform4fv(getCachedUniformLocation(program, name), 1, &value[0]);
}

void ShaderManager::setUniformMat4(GLuint program, const char* name, const glm::mat4& value){
    glUseProgram(program);
    glUniformMatrix4fv(getCachedUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
}

void ShaderManager::setUniformTex2D(GLuint program, const char* name, const GLuint texID, const GLuint unitIndex){
    glUseProgram(program);

    glActiveTexture(unitIndex);
    glBindTexture(GL_TEXTURE_2D, texID);

    GLint boundTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
    // std::cout << "Bound texture ID: " << boundTexture << ", Expected: " << texID << std::endl;

    if (boundTexture != texID) {
        std::cerr << "TEXTURE BINDING FAILED!" << std::endl;
    }
    glUniform1i(getCachedUniformLocation(program, name), unitIndex); // 0 = GL_TEXTURE0
}