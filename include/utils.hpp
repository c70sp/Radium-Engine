#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <glad/glad.h>
#include "./structs.hpp"

namespace utils{
    std::string loadFileAsString(const std::string& filename);
    
    void checkGLError(const std::string& location);

    float getRandomFloat(float min, float max);
    int getRandomInt(int min, int max);
    float getDistance(Point p1, Point p2);
}

#endif // UTILS_HPP