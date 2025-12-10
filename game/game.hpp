#pragma once

#include "./../engine/engine.hpp"

class Game {
    private:


    public:
        Game(Engine& engine);
        void init();
        void update(float dt);
        void render();
        void shutdown();

    private:
        void renderScene();

        void setUpCameras();
        void checkKeys();
};