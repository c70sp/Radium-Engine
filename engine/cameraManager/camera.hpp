#pragma once
// C++ STL
#include <iostream>

// 3rd party libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera{
    public:
        Camera(float width, float height, const std::string& nameIn);

        void update();
        void updateView();
        void updateProjection();
        void updateProjectionOrtho();

        void translate(glm::vec3 movement);
        void translate(glm::vec3 localDir, float amount);
        void setPosition(glm::vec3 position);

        // Getters
        glm::mat4 getVP() const {return projectionMatrix * viewMatrix;}
        glm::vec3 getPosition() const {return position;}

        float fov = 45.0f;

        float pitch = 0.0f;
        float yaw = -90.0f;
        float roll = 0.0f;

        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, front));

        std::string name;

    private:
        void updateVectors();

        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        
        glm::vec3 direction = glm::vec3(0.0f);
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);

        float aspect;
        float nearPlane = 0.1f;
        float farPlane = 5000.0f;
};