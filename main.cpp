#include "engine/engine.hpp"
#include "editor/editor.hpp"
#include "game/game.hpp"

int main(int argc, char** argv){
    bool runEditor = false;

    if(argc > 1 && std::string(argv[1]) == "--editor") runEditor = true;

    // Engine engine(1920, 1140);
    Engine engine(1920, 1000);
    SceneManager sceneManager;

    GameCallbacks callbacks;

    if(runEditor){
        Editor editor(engine, sceneManager);
        
        callbacks.onInit        = [&]() {editor.init();};
        callbacks.onUpdate      = [&](float dt) {editor.update(dt);};
        callbacks.onRender      = [&]() {editor.render();};
        callbacks.onShutdown    = [&]() {editor.shutdown();};

    }else{
        Game game(engine);

        callbacks.onInit        = [&]() {game.init();};
        callbacks.onUpdate      = [&](float dt) {game.update(dt);};
        callbacks.onRender      = [&]() {game.render();};
        callbacks.onShutdown    = [&]() {game.shutdown();};
    }

    engine.run(callbacks);

    return 0;
}