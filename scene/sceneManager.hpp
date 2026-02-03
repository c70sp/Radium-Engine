#pragma once

#include <vector>

#include "scene.hpp"

/**
 * This one is purely for managing all of the scenes.
 * Like creating a scene and getting a scene.
 * This is NOT for:
 *  - manipulating individual objects in the scene or
 *  - the scene itself.
 */
class SceneManager{
    public:
        Scene* createScene(const std::string& name);

        Scene* getScene(uint32_t index);
        Scene* activeScene(){return &mScenes.at(mActiveScene);};

        void setActiveScene(uint32_t index);
        int mActiveScene = 0;
    private:
        std::vector<Scene> mScenes;
};