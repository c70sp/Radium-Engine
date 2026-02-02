#include "editorUI.hpp"

// EditorUI::EditorUI(){

// }

void EditorUI::createEditorUI(){
    std::cout << "EditorUI creation...\n";
}

void EditorUI::setCurrentSceneStruct(SceneInfo& sceneStruct){
    mCurrentSceneStruct = &sceneStruct;
}

void EditorUI::setCurrentScene(Scene& scene){
    mScene = &scene;
}

void EditorUI::imGuiSetup(GLFWwindow* window){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.WindowPadding = ImVec2(10, 10);   // spacing between items
        style.ItemSpacing = ImVec2(8, 8);       // spacing between internal elements
        style.ItemInnerSpacing = ImVec2(4, 4);  // padding inside normal windows
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void EditorUI::renderDockspace(){
    static bool dockspaceOpen = true;
    static bool optFullscreen = true;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;

    if(optFullscreen){
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        windowFlags |=
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }else{
        windowFlags |= ImGuiWindowFlags_NoCollapse;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // fully transparent

    ImGui::Begin("Docking Window", &dockspaceOpen, windowFlags);

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("File")){
            ImGui::MenuItem("New");
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Window")){
            if(ImGui::BeginMenu("Save Layout")){
                ImGui::MenuItem("Layout 1");
                ImGui::MenuItem("Layout 2");
                ImGui::MenuItem("Layout 3");
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Load Layout")){
                ImGui::MenuItem("Layout 1");
                ImGui::MenuItem("Layout 2");
                ImGui::MenuItem("Layout 3");
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::PopStyleVar(2);

    ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f));

    ImGui::End();
}

void EditorUI::startUIDraw(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderDockspace();

    // Examples
    ImGui::Begin("Properties");
    ImGui::Text("... properties of selected item ...");
    ImGui::End();

    ImGui::Begin("Entity Tree");
    ImGui::Text("... list of all scene entities ...");
    ImGui::End();

    ImGui::Begin("Project browser");
    ImGui::Text("... list of all assets, scripts, etc. ...");
    if(ImGui::Button("Spawn cube")){
        std::cout << "Spawning cube...\n";
        spawnCube();
    }
    ImGui::End();

    ImGui::Begin("Console");
    ImGui::Text("... logging ...");
    ImGui::End();

    // vvvvvvvvvvvvvvvvvvvv VIEWPORT vvvvvvvvvvvvvvvvvvvv
    ImGui::Begin("Viewport");
    ImVec2 size = ImGui::GetContentRegionAvail();
    if(viewportSize.x != size.x || viewportSize.y != size.y){
        viewportChanged = true;
        viewportSize.x = size.x;
        viewportSize.y = size.y;
    }
    ImGui::End();
    // ^^^^^^^^^^^^^^^^^^^^ VIEWPORT ^^^^^^^^^^^^^^^^^^^^
}

void EditorUI::endUIDraw(GLuint &colorTex){

    // vvvvvvvvvvvvvvvvvvvv VIEWPORT vvvvvvvvvvvvvvvvvvvv
    ImGui::Begin("Viewport");
    ImGui::Image((ImTextureID)(intptr_t)colorTex, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
    if(ImGui::IsItemHovered()){
        mouseInViewport = true;
    }else{
        mouseInViewport = false;
    }
    ImGui::End();
    // ^^^^^^^^^^^^^^^^^^^^ VIEWPORT ^^^^^^^^^^^^^^^^^^^^

    // =================

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    GLFWwindow* backup = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup);
};

void EditorUI::spawnCube(){
}