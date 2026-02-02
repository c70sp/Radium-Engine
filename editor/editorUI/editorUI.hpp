#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>


#include "./../engine/engine.hpp"
#include "./../scene/sceneStruts.hpp"
#include "./../scene/scene.hpp"

// Just for some safety. Prob. not needed ig
class Scene;

/**
 * This will contain the ENTIRE UI system.
 * The systems will be split into seperate files.
 * 
 * The Editor UI will be initialized directy by the editor after it is done initalizing.
 * The UI will be ImGUI with docking.
 * The UI will set variables.
 * These will be written into a scene file.
 * The rendering engine will read those scene files and construct the rendering.
 * These scene files will be cached and constantly checked for updates.
 * If one happens, it automatically updates the in-game object.
 * This will allow for real-time editing. 
 */
class EditorUI{
    public:
        // EditorUI();

        void imGuiSetup(GLFWwindow* window);
        void createEditorUI();

        void startUIDraw();
        void endUIDraw(GLuint &colorTex);

        void renderDockspace();

        // Sets the specific scene
        void setCurrentSceneStruct(SceneInfo& sceneStruct);
        // Sets the current scene instance (only set once at beginning, as there should only be one of them..)
        void setCurrentScene(Scene& scene);


        glm::vec2 viewportSize = glm::vec2(100, 100);
        bool viewportChanged = false;
        bool mouseInViewport = false;
        bool viewportDragActive = false;

    private:
        SceneInfo* mCurrentSceneStruct;
        Scene* mScene;

        void spawnCube();
};