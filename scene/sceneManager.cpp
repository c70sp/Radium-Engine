#include "sceneManager.hpp"

Scene* SceneManager::createScene(const std::string& name){
    Scene scene;
    mScenes.emplace_back(scene);
    return &mScenes.back();
}

Scene* SceneManager::getScene(uint32_t index){
    return &mScenes.at(index);
}