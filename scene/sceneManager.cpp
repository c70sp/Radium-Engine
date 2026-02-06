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

void SceneManager::addEventListeners(){
    ENGINE->getEventSystem().subscribe<SaveSceneEvent>([this](const SaveSceneEvent& e){
        sceneSerializer.saveScene(
                *activeScene(),
                "C:\\dev\\projects\\C++\\OpenGL\\Radium Engine\\.assets\\savedScenes\\save1.json");
    });
            
    ENGINE->getEventSystem().subscribe<LoadSceneEvent>([this](const LoadSceneEvent& e){
        mScenes.emplace_back();
        auto& scene = mScenes.back();
        setActiveScene(std::distance(mScenes.data(), &scene)); // TODO: <= rework the scene setting system. Or similar.
        sceneSerializer.loadScene(scene, "C:\\dev\\projects\\C++\\OpenGL\\Radium Engine\\.assets\\savedScenes\\save1.json");
    });
}