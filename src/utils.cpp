#define GLM_ENABLE_EXPERIMENTAL

#include "./../include/utils.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include "./../include/structs.hpp"

namespace utils{
    std::string loadFileAsString(const std::string& filename){
        std::string result = "";

        std::string line = "";
        std::ifstream myFile(filename.c_str());

        if(myFile.is_open()){
            while(std::getline(myFile, line)){
                result += line + "\n";
            }
            myFile.close();
        }

        return result;
    }

    void checkGLError(const std::string& location) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error at " << location << ": " << err << std::endl;
        }
    }

    float getRandomFloat(float min, float max) {
        std::random_device rd;  // Seed for the random number engine
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    int getRandomInt(int min, int max) {
        std::random_device rd;  // Seed for the random number engine
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    float getDistance(Point p1, Point p2){
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }
}