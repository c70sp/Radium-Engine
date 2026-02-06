#pragma once

#include <vector>

#include "scene.hpp"
#include "SceneSerializer/sceneSerializer.hpp"

/**
 * This one is purely for managing all of the scenes.
 * Like creating a scene and getting a scene.
 * This is NOT for:
 *  - manipulating individual objects in the scene or
 *  - the scene itself.
 */
class SceneManager{
    public:
        SceneManager(){addEventListeners();};

        Scene* createScene(const std::string& name);

        Scene* getScene(uint32_t index);
        Scene* activeScene(){return &mScenes.at(mActiveScene);};

        void setActiveScene(uint32_t index);
        uint32_t mActiveScene = 0;

        void addEventListeners();
    private:
        SceneSerializer sceneSerializer;

        std::vector<Scene> mScenes;
};