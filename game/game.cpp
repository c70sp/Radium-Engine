#include "game.hpp"

Game::Game(Engine& engine){
    std::cout << "Running in game-mode!\n";

    auto& sm = ENGINE->getShaderManager();
    sm.compileAllShaders();

    auto& cm = ENGINE->getCollisionManager();
    cm.init();
}

void Game::setUpCameras(){
    auto& camRef = ENGINE->getCameraManager();

    int width = ENGINE->window.mWidth;
    int height = ENGINE->window.mHeight;
    camRef.createCamera(width, height, "playerCamera"); // auto sets active cam
}


void Game::init(){
    setUpCameras();

    auto& sm = ENGINE->getShaderManager();
    auto& tm = ENGINE->getTextManager();
    ProgramInfo textProg = sm.getProgram("text", "v_UItext.glsl", "f_UItext.glsl");
    tm.loadFont("C:/dev/projects/C++/OpenGL/manIdk/.assets/fonts/Roboto-VariableFont_wdth,wght.ttf", textProg.id, 16);
}

void Game::checkKeys(){
    auto& km = ENGINE->getKeyManager();

    if(km.isKeyDown(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(ENGINE->getWindow(), true);
}

void Game::update(float dt){
    auto cam = ENGINE->getCameraManager().getActiveCamera();
    auto& cm = ENGINE->getCollisionManager();
    checkKeys();
}

void Game::renderScene(){
    auto& renderer = ENGINE->getRenderer();
    auto& sm = ENGINE->getShaderManager();
    
    auto cam = ENGINE->getCameraManager().getActiveCamera();
    cam->update();
    glm::mat4 vp = cam->getVP();

    // Draw static objects
    // for(auto& object : world.staticMeshes){
        // renderer.drawMesh(object, sm, vp);
    // }
}

void Game::render(){
    renderScene();
}

void Game::shutdown(){

}