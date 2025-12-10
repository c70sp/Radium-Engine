// C++ STL
#include <fstream>
#include <string>
#include <iostream>

#include "glad/glad.h"

#include "common.hpp"

std::string loadFileAsString(const std::string& path){
    std::string result = "";
    
    std::string line = "";
    std::ifstream myFile(path.c_str());

    if(myFile.is_open()){
        while(std::getline(myFile, line)){
            result += line + "\n";
        }
        myFile.close();
    }

    return result;
}

void checkGLError(const std::string& location){
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
    }
}

glm::vec3 worldToScreen(const glm::vec3& position, const glm::mat4& VPmat, int screenWidth, int screenHeight){
    glm::vec4 clipSpacePos = VPmat * glm::vec4(position, 1.0f);

    // perspective divide -> NDC
    if (clipSpacePos.w == 0.0f) return glm::vec3(-1.0f); // avoid divide by 0
    glm::vec3 ndc = glm::vec3(clipSpacePos) / clipSpacePos.w;

    // convert to screen space (0..width, 0..height)
    glm::vec2 screenPos;
    screenPos.x = (ndc.x * 0.5f + 0.5f) * screenWidth;
    screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * screenHeight; // flip Y

    return glm::vec3(screenPos, ndc.z);
}