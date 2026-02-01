#pragma once

#include <vector>

#include "scene.hpp"

class SceneManager{
    public:
        Scene* createScene(const std::string& name);

        Scene* getScene(uint32_t index);

    private:
        std::vector<Scene> mScenes;
};