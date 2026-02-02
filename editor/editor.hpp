#pragma once

#include <iostream>
#include <set>

#include "./../engine/engine.hpp"
#include "./../scene/sceneManager.hpp"
#include "./editorUI/editorUI.hpp"

#include "./../staticConfig.hpp"

struct MouseData{
    std::set<int> heldButtons;

    glm::vec2 mousePos;
    glm::vec2 lastMousePos;
    glm::vec2 delta;

    glm::vec2 offset; // scrolling, Y is important
};

struct KeyData{
    std::set<int> heldButtons;
};

class Editor{
    public:
        Editor(Engine& engine, SceneManager& sceneManager);
        void init();
        void update(float dt);
        void render();
        void shutdown();

    private:
        SceneManager& mSceneManager;
        Engine& mEngine;

        EditorUI editorUI;

        GLuint mFBO, mColorTex, mDepthRBO;

        Scene* mScene1;
        // SceneStruct scene1;
        // Entity* ent;


        //! vvvvvvvvvvvvvvv editorFBO.cpp vvvvvvvvvvvvvvv
        void createFBO();
        void drawAllToFBO();
        
        void resizeViewport();
        void resizeFBO();
        void recalcCameraAspect();
        //! ^^^^^^^^^^^^^^^ editorFBO.cpp ^^^^^^^^^^^^^^^

        //! vvvvvvvvvvvvvvv editorInputHandling.cpp vvvvvvvvvvvvvvv
        void createEventListeners();

        void handleViewportInputs();

        void handleMouseButton(const MouseButtonEvent& e);
        void handleMouseMove(const MouseMoveEvent& e);
        void handleMouseScroll(const MouseScrollEvent& e);

        void handleKeyPress(const KeyPressEvent& e);
        //! ^^^^^^^^^^^^^^^ editorInputHandling.cpp ^^^^^^^^^^^^^^^

        MouseData mouseData;
        KeyData keyData;
};