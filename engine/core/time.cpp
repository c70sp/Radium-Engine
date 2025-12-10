#include "time.hpp"

Time::Time(){
    lastFrame = std::chrono::high_resolution_clock::now();
    delta = 0.0f;
}

void Time::update(){
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> diff = now - lastFrame;
    delta = diff.count();
    lastFrame = now;
}

float Time::deltaTime() const{
    return delta;
}