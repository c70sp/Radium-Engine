#include "raycast.hpp"

// Möller–Trumbore intersection algorithm
bool RayCast::rayIntersectsTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayDir,
                               const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                               float& outDistance)
{
    // std::cout << "1" << std::endl;
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(rayDir, edge2);
    float a = glm::dot(edge1, h);

    // std::cout << "2" << std::endl;
    if(fabs(a) < EPSILON) return false; // parallel

    // std::cout << "3" << std::endl;
    float f = 1.0f / a;
    glm::vec3 s = rayOrigin - v0;
    float u = f * glm::dot(s, h);
    if(u < 0.0f || u > 1.0f) return false;

    // std::cout << "4" << std::endl;
    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(rayDir, q);
    if(v < 0.0f || u + v > 1.0f) return false;

    // std::cout << "5" << std::endl;
    float t = f * glm::dot(edge2, q);
    if(t > EPSILON){
        outDistance = t;
        return true;
    }

    return false;
}

glm::vec3 RayCast::castRay(const glm::vec3& rayOrigin, const glm::vec3& rayDir, std::vector<GLfloat> vertices, std::vector<GLuint> indices){
    // ======================================================================================================
    float maxCheckDistance = 10.0f; // how far around the player to check (in world units)
    float cellSize = 1.0f;
    float width = 1000.0f;
    float depth = 1000.0f;

    glm::vec3 hitPos;

    int minX = std::max(0, (int)floor((rayOrigin.x - maxCheckDistance) / cellSize));
    int maxX = std::min((int)(width - 1), (int)ceil((rayOrigin.x + maxCheckDistance) / cellSize));
    
    int minZ = std::max(0, (int)floor((rayOrigin.z - maxCheckDistance) / cellSize));
    int maxZ = std::min((int)(depth - 1), (int)ceil((rayOrigin.z + maxCheckDistance) / cellSize));
    
    float closestHit = FLT_MAX;
    bool hit = false;
    
    auto getVertex = [&](unsigned int i) -> glm::vec3 {
    return glm::vec3(
        vertices[i * 3 + 0],
        vertices[i * 3 + 1],
        vertices[i * 3 + 2]
    );};
        
    for (int z = minZ; z <= maxZ; ++z) {
        for (int x = minX; x <= maxX; ++x) {
            // compute vertex indices
            int i0 = z * (width + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (width + 1);
            int i3 = i2 + 1;
            
            glm::vec3 v0 = getVertex(i0);
            glm::vec3 v1 = getVertex(i1);
            glm::vec3 v2 = getVertex(i2);
            glm::vec3 v3 = getVertex(i3);
            
            float dist;

            // Triangle 1
            if (rayIntersectsTriangle(rayOrigin, rayDir, v0, v1, v2, dist)) {
                if (dist < closestHit) {
                    closestHit = dist;
                    hit = true;
                }
            }

            // Triangle 2
            if (rayIntersectsTriangle(rayOrigin, rayDir, v3, v1, v2, dist)) {
                if (dist < closestHit) {
                    closestHit = dist;
                    hit = true;
                }
            }
        }
    }

    if(hit){
        hitPos = rayOrigin + rayDir * closestHit;
        return hitPos;
    }else{
        return glm::vec3(0.0f);
    }
}