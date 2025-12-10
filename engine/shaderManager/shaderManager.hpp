#pragma once

// 3rd party libs
#include <glad/glad.h>

// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

struct ShaderInfo{
    std::string name;
    GLuint id;
    GLuint type;
};

struct ProgramInfo{
    std::string name;
    GLuint id;
    std::vector<ShaderInfo> programShaders;
};

class ShaderManager{
    public:
        // ShaderManager(); Problem: er will auf opengl funk. zugreifen die noch garnicht existieren (pre init...)
        // Kann ich später fixen

        template<typename... ShaderNames>
        GLuint getProgram(const std::string& programName, const ShaderNames&... filenames){
            std::vector<std::string> shadersForProgram = {filenames...};

            ProgramInfo prog = getShaderProgram(shadersForProgram, programName);

            if (prog.id == 0) {
                std::cerr << "[ShaderManager] Fatal: Program \"" << programName << "\" could not be created (missing or invalid shaders)\n";
            }

            return prog.id;
        };


        void compileAllShaders();

        // Uniform setters
        void setUniformInt(GLuint program, const char* name, const int value);
        void setUniformFloat(GLuint program, const char* name, const float value);
        void setUniformVec3(GLuint program, const char* name, const glm::vec3& vector);
        void setUniformVec4(GLuint program, const char* name, const glm::vec4& vector);
        void setUniformMat4(GLuint program, const char* name, const glm::mat4& matrix);

        // Uniform texture setters
        void setUniformTex2D(GLuint program, const char* name, const GLuint texID, const GLuint unitIndex);
    private:
        std::vector<ShaderInfo> mShaders;
        std::vector<ProgramInfo> mShaderPrograms;
        
        GLuint getShaderType(std::string shaderName);
        
        ProgramInfo* findExistingProgram(std::vector<ShaderInfo>, const std::string& name);
        ProgramInfo createNewProgram(std::vector<ShaderInfo> shaders, const std::string& name);
        ProgramInfo getShaderProgram(std::vector<std::string> shaderNames, const std::string& name);
        
        GLuint compileShader(GLuint type, const std::string& source, const std::string& name = "");

        GLint getCachedUniformLocation(GLuint program, const std::string& name);
        std::unordered_map<GLuint, std::unordered_map<std::string, GLint>> uniformCache;
};