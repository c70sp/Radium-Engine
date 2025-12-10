#pragma once

// C++ STL
#include <vector>
#include <iostream>

// 3rd party lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class RayCast{
    public:
        bool rayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
                                   const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                                   float& outDistance);

        glm::vec3 castRay(const glm::vec3& rayOrigin, const glm::vec3& rayDir, std::vector<GLfloat> vertices, std::vector<GLuint> indices);

    private:
};