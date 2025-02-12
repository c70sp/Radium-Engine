#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <glad/glad.h>
#include "./structs.hpp"
#include "./../include/radium_export.hpp"

namespace utils{
    RADIUM_API std::string loadFileAsString(const std::string& filename);
    
    RADIUM_API void checkGLError(const std::string& location);

    RADIUM_API float getRandomFloat(float min, float max);
    RADIUM_API int getRandomInt(int min, int max);
    RADIUM_API float getDistance(Point p1, Point p2);
}

#endif // UTILS_HPP