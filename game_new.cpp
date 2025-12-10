#include "game.hpp"

Game::Game(Engine& engine) 
    : managers{
        engine.getRenderer(),
        engine.getMeshManager(), 
        engine.getShaderManager(),
        engine.getCameraManager(),
        engine.getTextureManager(),
        engine.getTextManager(),
        engine.getUIManager()
    } {
}

void Game::init() {
    setupShaders();
    setupCameras(); 
    setupWorld();
    setupUI();
}

void Game::setupShaders() {
    managers.shaderManager.compileAllShaders();
    shaders.worldShader = managers.shaderManager.getProgram("world", "v_vert.glsl", "f_frag.glsl");
    shaders.uiShader = managers.shaderManager.getProgram("ui", "v_UIvert.glsl", "f_UIfrag.glsl");
    shaders.uiTextShader = managers.shaderManager.getProgram("uiText", "v_UItext.glsl", "f_UItext.glsl");
}

void Game::setupCameras() {
    auto& window = ENGINE->window;
    state.playerCamera = managers.cameraManager.createCamera(window.mWidth, window.mHeight, "playerCamera");
    state.uiCamera = managers.cameraManager.createCamera(window.mWidth, window.mHeight, "uiCam");
}

void Game::setupWorld() {
    std::string path = "C:/dev/projects/C++/OpenGL/manIdk/.assets/objects/SpaceShip1.obj";
    managers.meshManager.loadModel(path);

    state.worldMesh = managers.meshManager.getModel("Spaceship");
    state.worldMesh.program = shaders.worldShader;

    state.interior = managers.meshManager.getModel("Interior");
    state.interior.program = shaders.worldShader;

    textures.spaceship = managers.textureManager.loadTexture("C:/dev/projects/C++/OpenGL/manIdk/.assets/textures/8x8Palette.png");

    // ^^^^^^^^^^^^^^^ ASSIMP LOADER ^^^^^^^^^^^^^^^


    state.worldMesh.transforms = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    
    // misc.player.setTerrain(vertices, indices);
}

void Game::setupUI() {
    managers.textManager.loadFont("C:/dev/projects/C++/OpenGL/manIdk/.assets/fonts/Roboto-VariableFont_wdth,wght.ttf", shaders.uiTextShader, 48);

    ui.hungerBar = managers.uiManager.create<UIRect>(
        glm::vec2(0, 0), glm::vec2(300, 50),
        glm::vec4(1, 1, 0, 1), shaders.uiShader
    );
    
    ui.uiTexture = managers.textureManager.loadTexture("C:/dev/projects/C++/OpenGL/manIdk/.assets/objects/red.png");
}

void Game::update(float dt) {
    updateWorld(dt);
    updateCamera();
    updateUI(dt);
}

void Game::updateWorld(float dt) {
    // misc.spaceship.update();
}

void Game::updateCamera() {
    // state.playerCamera->setPosition(glm::vec3(misc.spaceship.position));
    state.playerCamera->update();
    state.mainVP = state.playerCamera->getVP();
}

void Game::updateUI(float dt) {
    // Animate UI
    glm::vec2 size = ui.hungerBar->size;
    size.x -= 25.0f * dt;
    ui.hungerBar->setSize(size);
}

void Game::render() {
    managers.renderer.beginFrame();
    renderWorld();
    renderUI();
    managers.renderer.endFrame();
}

void Game::renderWorld() {
    // Set world shader uniforms
    managers.shaderManager.setUniformMat4(shaders.worldShader, "u_ModelMatrix", state.worldMesh.transforms);
    managers.shaderManager.setUniformMat4(shaders.worldShader, "u_VP", state.mainVP);
    
    managers.shaderManager.setUniformTex2D(shaders.worldShader, "u_texture", textures.spaceship->id);
    managers.renderer.drawMesh(state.worldMesh);
    managers.renderer.drawMesh(state.interior);
}

void Game::renderUI() {
    glDisable(GL_DEPTH_TEST);
    state.uiCamera->updateProjectionOrtho();
    auto vp = state.uiCamera->getVP();
    
    managers.shaderManager.setUniformMat4(shaders.uiTextShader, "u_VP", vp);
    
    managers.shaderManager.setUniformMat4(shaders.uiShader, "u_VP", vp);
    managers.shaderManager.setUniformTex2D(shaders.uiShader, "u_diffuse", ui.uiTexture->id);
    managers.renderer.drawUI(managers.uiManager);
    
    managers.textManager.renderText("Test test yo :)", glm::vec2(0.0f), 1.0f, glm::vec3(0, 1, 0));
    managers.textManager.renderText("I have SPACESHIP!!!!!", glm::vec2(100.0f, 100.0f), 1.0f, glm::vec3(1, 1, 1));
    managers.textManager.renderText("!!!!11!!", glm::vec2(350.0f, 150.0f), 1.0f, glm::vec3(1, 1, 1));
    managers.textManager.renderText("!!!!11!!", glm::vec2(1000.0f, 200.0f), 1.0f, glm::vec3(1, 1, 1));
    managers.textManager.renderText("!!!!11!!", glm::vec2(600.0f, 800.0f), 1.0f, glm::vec3(1, 1, 1));
    managers.textManager.renderText("!!!!11!!", glm::vec2(150.0f, 500.0f), 1.0f, glm::vec3(1, 1, 1));
    managers.textManager.renderText("!!!!11!!", glm::vec2(800.0f, 300.0f), 1.0f, glm::vec3(1, 1, 1));


    glEnable(GL_DEPTH_TEST);
}

void Game::shutdown() {
    // Cleanup if needed
}