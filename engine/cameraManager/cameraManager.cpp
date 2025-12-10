#include "cameraManager.hpp"

std::shared_ptr<Camera> CameraManager::createCamera(int width, int height, const std::string& name){
    auto camera = std::make_shared<Camera>(width, height, name);
    cameras.push_back(camera);

    if(!activeCamera) activeCamera = camera;

    return camera;
}

void CameraManager::setActiveCamera(const std::string& name){
    for(auto& cam : cameras){
        if(cam->name == name){
            activeCamera = cam;
            return;
        }
    }
    std::cerr << "Camera with name: " << name << " not found" << std::endl;
}

std::shared_ptr<Camera> CameraManager::getCamera(const std::string& name) const{
    for(auto& cam : cameras){
        if(cam->name == name){
            return cam;
        }
    }

    return nullptr;
}

std::shared_ptr<Camera> CameraManager::getActiveCamera() const{
    return activeCamera;
}

const std::vector<std::shared_ptr<Camera>>& CameraManager::getAllCameras() const{
    return cameras;
}