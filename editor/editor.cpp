#include "editor.hpp"

Editor::Editor(Engine& engine, Scene& scene) : mEngine(engine), mScene(scene){
    std::cout << "Running in editor-mode!\n";
    std::cout << &engine << " | " << &scene << "\n";
}

void Editor::init(){
    // Renderer needs access to mesh and shader manager. To avoid circular dependancies, I do it like this.
    // Not pretty, but it does the job.
    // mEngine.getRenderer().setMeshManagerRef(mEngine.getMeshManager());
    // mEngine.getRenderer().setShaderManagerRef(mEngine.getShaderManager());

    ent = mScene.createEntity(scene1);
    TransformComponent trans;
    MeshComponent mesh;
    MaterialComponent mat;
    mScene.addComponent<TransformComponent>(*ent, trans);
    mScene.addComponent<MeshComponent>(*ent, mesh);
    mScene.addComponent<MaterialComponent>(*ent, mat);

    auto trans2 = mScene.getComponent<TransformComponent>(*ent);
    std::cout << trans2->pos.r << std::endl;

    editorUI.imGuiSetup(mEngine.window.mWindow);
    editorUI.createEditorUI();

    // Explodes if not set :( (well obviously)
    // If I change shit here, I should prob. implement a simple nullptr check in EditorUI as I no longer use references here.
    editorUI.setCurrentScene(mScene);
    editorUI.setCurrentSceneStruct(scene1);

    createFBO();
}

void Editor::update(float dt){
    if(mEngine.getKeyManager().isKeyDown(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(mEngine.window.mWindow, true);
}

void Editor::drawAllToFBO(){
    std::vector<uint32_t> drawableEntities;

    
    for (auto& e : scene1.entities) {

        std::cout << "Has mesh: " << scene1.meshes.contains(e.id) << std::endl;
        std::cout << "Has trans: " << scene1.transforms.contains(e.id) << std::endl;
        std::cout << "Has mat: " << scene1.materials.contains(e.id) << std::endl;

        if (scene1.meshes.contains(e.id) &&
            scene1.transforms.contains(e.id) &&
            scene1.materials.contains(e.id))
        {
            drawableEntities.push_back(e.id);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
    
    for (uint32_t id : drawableEntities) {
        std::cout << "Drawing id: " << id << "\n";
        auto& meshComp  = scene1.meshes[id];
        auto& transComp = scene1.transforms[id];
        auto& matComp   = scene1.materials[id];

        auto& mesh = mEngine.getMeshManager().meshStorage.at(meshComp.meshID);
        
        mEngine.getRenderer().drawMesh2(mesh, transComp, matComp, mEngine.getShaderManager());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Editor::render(){
    editorUI.startUIDraw();
    drawAllToFBO();
    // editorUI.endUIDraw();
}

void Editor::shutdown(){

}