#pragma once

#include <iostream>
#include <string>
#include <random>

#include "./../engine/engine.hpp"
#include "./../../scene/sceneStruts.hpp"

// Move this to editorFunctions
// With a deticated file for the function spawnEntity
class EditorActions{
    public:
        void spawnEntity();
        float randNum();

    private:
};