#include "sceneManager.hpp"

Scene* SceneManager::createScene(const std::string& name){
    mScenes.emplace_back();
    return &mScenes.back();
}

Scene* SceneManager::getScene(uint32_t index){
    return &mScenes.at(index);
}

void SceneManager::setActiveScene(uint32_t index){
    mActiveScene = index;
}