#pragma once
#include <chrono>

class Time{
    public:
        Time();
        void update();
        float deltaTime() const;

    private:
        std::chrono::high_resolution_clock::time_point lastFrame;
        float delta;
};