#include "editor.hpp"

Editor::Editor(Engine& engine, SceneManager& sceneManager) : mEngine(engine), mSceneManager(sceneManager){
    std::cout << "Running in editor-mode!\n";
    std::cout << &engine << " | " << &sceneManager << "\n";
}

void Editor::init(){
    // Renderer needs access to mesh and shader manager. To avoid circular dependancies, I do it like this.
    // Not pretty, but it does the job.
    // mEngine.getRenderer().setMeshManagerRef(mEngine.getMeshManager());
    // mEngine.getRenderer().setShaderManagerRef(mEngine.getShaderManager());

    // ent = mScene.createEntity(scene1);
    // TransformComponent trans;
    // MeshComponent mesh;
    // MaterialComponent mat;
    // mScene.addComponent<TransformComponent>(*ent, trans);
    // mScene.addComponent<MeshComponent>(*ent, mesh);
    // mScene.addComponent<MaterialComponent>(*ent, mat);

    // auto trans2 = mScene.getComponent<TransformComponent>(*ent);
    // std::cout << trans2->pos.r << std::endl;

    editorUI.imGuiSetup(mEngine.window.mWindow);
    editorUI.createEditorUI();

    // Explodes if not set :( (well obviously)
    // If I change shit here, I should prob. implement a simple nullptr check in EditorUI as I no longer use references here.
    // editorUI.setCurrentScene(mScene);
    // editorUI.setCurrentSceneStruct(scene1);

    mEngine.getMeshManager().loadModel("C:\\dev\\projects\\C++\\OpenGL\\Radium Engine\\.assets\\objects\\SpaceShip1 - Export.obj");
    Mesh3D test = mEngine.getMeshManager().getModel("Hull");

    mEngine.getShaderManager().compileAllShaders();
    GLuint basicProgram = mEngine.getShaderManager().getProgram("basic", "v_vert.glsl", "f_frag.glsl");

    mScene1 = mSceneManager.createScene("Scene1");
    Entity* ent = mScene1->createEntity("ent");

    TransformComponent trans;
    trans.pos.x = 5;
    trans.pos.z = -30;
    trans.scale = glm::vec3(1);
    trans.rot.y = 45;
    trans.rot.x = -75;
    
    MeshComponent mesh;
    mesh.meshID = test.id;
    mesh.meshName = test.name;

    MaterialComponent mat;
    mat.programID = basicProgram;
    mat.programName = "basic";
    mScene1->addComponent(*ent, trans);
    mScene1->addComponent(*ent, mesh);
    mScene1->addComponent(*ent, mat);

    std::cout << mScene1->getComponent<TransformComponent>(*ent).pos.x << "\n";

    createFBO();


    ENGINE->getCameraManager().createCamera(editorUI.viewportSize.x, editorUI.viewportSize.y, "viewportCamera");


    createEventListeners();
}

void Editor::update(float dt){
    if(editorUI.viewportDragActive) handleViewportInputs();
}

void Editor::drawAllToFBO(){
    std::vector<uint32_t> drawableEntities;

    
    for (auto& e : mScene1->entities) {

        if constexpr (staticConfig::debugging){
            std::cout << "Has mesh: " << mScene1->meshes.contains(e.id) << std::endl;
            std::cout << "Has trans: " << mScene1->transforms.contains(e.id) << std::endl;
            std::cout << "Has mat: " << mScene1->materials.contains(e.id) << std::endl;
        }

        if (mScene1->meshes.contains(e.id) &&
            mScene1->transforms.contains(e.id) &&
            mScene1->materials.contains(e.id))
        {
            if constexpr (staticConfig::debugging) std::cout << "try pushback\n";
            drawableEntities.push_back(e.id);
        }else{
            if constexpr (staticConfig::debugging) std::cout << "no pushback\n";
        }
    }

    glDisable(GL_SCISSOR_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    glViewport(0, 0, editorUI.viewportSize.x, editorUI.viewportSize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (uint32_t id : drawableEntities) {
        if constexpr (staticConfig::debugging) std::cout << "Drawing id: " << id << "\n";
        if(!mScene1->transforms.contains(id) &&
           !mScene1->meshes.contains(id) &&
           !mScene1->materials.contains(id))
        {
            std::cerr << "Tried to retrive non-existing compontent! Element ID: " << id << "\n";
            continue;
        }else{
            auto& meshComp  = mScene1->meshes[id];
            auto& transComp = mScene1->transforms[id];
            auto& matComp   = mScene1->materials[id];
    
            auto& mesh = mEngine.getMeshManager().meshStorage.at(meshComp.meshID);

            auto cam = ENGINE->getCameraManager().getActiveCamera();
            cam->update();
            glm::mat4 vp = cam->getVP();

            mEngine.getRenderer().drawMesh2(mesh, transComp, matComp, mEngine.getShaderManager(), vp);
        }
        
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ENGINE->mWindowSize.x, ENGINE->mWindowSize.y);
}

void Editor::render(){
    editorUI.startUIDraw();

    if(editorUI.viewportChanged) resizeViewport();

    drawAllToFBO();

    GLuint* mColorTexPointer = &mColorTex;
    editorUI.endUIDraw(*mColorTexPointer);

    mouseData.delta = glm::vec2(0);
    mouseData.offset = glm::vec2(0);
}

void Editor::shutdown(){

}