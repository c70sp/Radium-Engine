#include "camera.hpp"

Camera::Camera(float width, float height, const std::string& nameIn){
    aspect = 640 / 480; // TODO: should be width / height, but hardcoded now for debugging

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    name = nameIn;

    updateProjection();
    updateView();
}

void Camera::updateView(){
    // for mouse movement, could go into seperate methode
    
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    
    pitch = glm::clamp(pitch, -89.0f, 89.0f); // just for extra safety

    front = glm::normalize(direction);

    updateVectors();

    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::updateProjection(){
    fov = glm::clamp(fov, 20.0f, 120.0f);
    projectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

void Camera::updateProjectionOrtho(){
    projectionMatrix = glm::orthoRH(0.0f, 1280.0f, 0.0f, 720.0f, nearPlane, farPlane);
}

void Camera::translate(glm::vec3 movement){
    position += movement;
}

void Camera::translate(glm::vec3 localDir, float amount){
    glm::vec3 worldMovement = right * localDir.x + up * localDir.y + front * localDir.z;

    position += worldMovement * amount;
}

void Camera::updateVectors(){
    front   = glm::normalize(direction);
    right   = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); // vec3 = worldUp constant
    up      = glm::normalize(glm::cross(right, front));
}

void Camera::setPosition(glm::vec3 positionIn){
    position = positionIn;
}

void Camera::update(){
    updateView();
    updateProjection();
}