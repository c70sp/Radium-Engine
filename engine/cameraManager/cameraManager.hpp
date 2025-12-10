#pragma once

// C++ STL
#include <memory>
#include <vector>
#include <string>

// 3rd party libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Own libs
#include "camera.hpp"

class CameraManager{
    public:
        std::shared_ptr<Camera> createCamera(int width, int height, const std::string& name);

        void setActiveCamera(const std::string& name);
        
        std::shared_ptr<Camera> getActiveCamera() const;
        std::shared_ptr<Camera> getCamera(const std::string& name) const;
        const std::vector<std::shared_ptr<Camera>>& getAllCameras() const;

    private:
        std::vector<std::shared_ptr<Camera>> cameras;
        std::shared_ptr<Camera> activeCamera;
};